#include "../Cuda/ImageView.h"

#include "mph/tuple_helpers.h"
#include "mph/qualifier_helpers.h"

#include "ScriptTraits.h"
#include "ScriptTraitTfms.h"
#include "ArgParser.h"

#include <tuple>
#include <string>
#include <cstdio>
#include <memory>
#include <type_traits>

namespace Script
{
	// Helper for python arg expanders
	template <typename T>
	struct ParserArg {};

	template <template<typename> class T, typename C>
	struct ParserArg<T<C>>
	{
		using ArgTuple = std::tuple<Script::ObjectType const**>;
		static ArgTuple argTuple(Script::ObjectType const** argPtr)
		{
			return std::make_tuple(argPtr);
		}

		// TODO: Move to compile-time strings (maybe library)
		static const char* argString()
		{
			static const char parse_type[] = "O";
			return parse_type;
		};
	};

	template <typename C>
	struct ParserArg<Image<C>>
	{
		using ArgTuple = std::tuple<PyTypeObject*, Script::ArrayType const**>;
		static ArgTuple argTuple(Script::ArrayType const** argPtr)
		{
			return std::make_tuple(&PyArray_Type, argPtr);
		}

		static const char* argString()
		{
			static const char parse_type[] = "O!";
			return parse_type;
		};
	};

	template <typename C>
	struct ParserArg<ImageRef<C>>
	{
		using ArgTuple = std::tuple<PyTypeObject*, Script::ArrayType const**>;
		static ArgTuple argTuple(Script::ArrayType const** argPtr)
		{
			return std::make_tuple(&PyArray_Type, argPtr);
		}

		static const char* argString()
		{
			static const char parse_type[] = "O!";
			return parse_type;
		};
	};

	// Helper struct to handle returning 0,1, or tuple of N outputs
	template <std::size_t N>
	struct StoreHelper
	{
	private:
		template <typename... ScrOuts, std::size_t... Is>
		static void store_out_impl(Script::ObjectType*& scriptTuple, const std::tuple<ScrOuts...>& outputs, mph::index_sequence<Is...>)
		{
			(void)std::initializer_list<int>
			{
				(PyTuple_SetItem(scriptTuple, Is, reinterpret_cast<Script::ObjectType*>(std::get<Is>(outputs))), void(), 0)...
			};
		}

	public:
		template <typename... ScrOuts>
		static Script::ObjectType* store_out(const std::tuple<ScrOuts...>& outputs)
		{
			static_assert(sizeof... (ScrOuts) == N, "HIP_COMPILE: Output argument selector size mismatch");

			Script::ObjectType* scriptOut = PyTuple_New(N);
			store_out_impl(scriptOut, outputs, mph::make_index_sequence<sizeof... (ScrOuts)>{});

			return scriptOut;
		}
	};

	template <>
	struct StoreHelper<1>
	{
		template <typename... ScrOuts>
		static Script::ObjectType* store_out(const std::tuple<ScrOuts...>& outputs)
		{
			static_assert(sizeof... (ScrOuts) == 1, "HIP_COMPILE: Output argument selector size mismatch");
			return reinterpret_cast<Script::ObjectType*>(std::get<0>(outputs));
		}
	};

	template <>
	struct StoreHelper<0>
	{
		template <typename... ScrOuts>
		static Script::ObjectType* store_out(const std::tuple<ScrOuts...>& outputs)
		{
			static_assert(sizeof... (ScrOuts) == 0, "HIP_COMPILE: Output argument selector size mismatch");
			return Py_None;
		}
	};



	template <typename Derived, typename... Layout>
	struct PyArgParser : public ArgParser<Derived, Layout...>
	{
		using BaseParser = ArgParser<Derived, Layout...>;

		using typename BaseParser::ArgError;

		// Argument type layout alias (e.g. std::tuple<OutParam<Image<Deferred>>,...>)
		using typename BaseParser::ArgLayout;

		// Script argument type layout (e.g. std::tuple<const PyArrayObject*,...>
		using typename BaseParser::ScriptTypes;
		using typename BaseParser::ScriptPtrs;

		// Concrete type layouts (e.g. std::tuple<PyObject*,...>)
		using typename BaseParser::ArgTypes;

		// IO-type stripped layout subsets (e.g. OutParam<Image<Deferred>> -> Image<Deferred>)
		using typename BaseParser::OutTypeLayout;
		using typename BaseParser::InTypeLayout;
		using typename BaseParser::OptTypeLayout;

		// IO Selectors
		using typename BaseParser::OutputSel;
		using typename BaseParser::InputSel;
		using typename BaseParser::OptionalSel;


		static void load(ScriptPtrs& scriptPtrs, Script::ObjectType*& scriptOut, Script::ObjectType* scriptIn)
		{
			mph::tuple_fill_value(mph::tuple_deref(scriptPtrs), nullptr);

			auto inPtrs = InputSel::select(scriptPtrs);
			auto optPtrs = OptionalSel::select(scriptPtrs);

			//  Make parse_string and expanded arg tuple for PyParse_Tuple
			const std::string parseStr = make_parse_str<InTypeLayout>() + "|" + make_parse_str<OptTypeLayout>();
			auto parseArgs = std::tuple_cat(expand_parse_args<InTypeLayout>(inPtrs), expand_parse_args<OptTypeLayout>(optPtrs));

			//  Call PyParse_Tuple return on error
			if ( !parse_script(scriptIn, parseStr, parseArgs) )
				throw ArgError("PyArg_ParseTuple failed");

			// TODO: Check input image dimension info
		}


		static void store(ScriptPtrs& scriptPtrs, Script::ObjectType*& scriptOut, Script::ObjectType* scriptIn)
		{
			auto outPtrs = OutputSel::select(scriptPtrs);
			auto outRefs = mph::tuple_deref(outPtrs);

			scriptOut = StoreHelper<OutputSel::seq::size()>::store_out(outRefs);
		}

	public:
		//////////////////////////////////
		// Basic type conversions
		// TODO: Fix const-ness issues for Python converters in concrete types
		template <typename T>
		static void convert_impl(T& out, const Script::ObjectType* inPtr)
		{
			out = Converter::toNumeric<T>(const_cast<Script::ObjectType*>(inPtr));
		}

		template <typename T>
		static void convert_impl(Script::ObjectType*& outPtr, const T& in)
		{
			outPtr = Converter::fromNumeric(in);
		}

		static void convert_impl(std::string& out, const Script::ObjectType* inPtr)
		{
			out = Converter::toString(const_cast<Script::ObjectType*>(inPtr));
		}

		template <typename T>
		static void convert_impl(Script::ObjectType*& outPtr, const std::string& in)
		{
			outPtr = Converter::fromString(in);
		}

		// Vector conversions
		template <typename T>
		static void convert_impl(Vec<T>& out, const Script::ObjectType* inPtr)
		{
			out = Converter::toVec<T>(const_cast<Script::ObjectType*>(inPtr));
		}

		template <typename T>
		static void convert_impl(Script::ObjectType*& outPtr, const Vec<T>& in)
		{
			outPtr = Converter::fromVec(in);
		}


		// Concrete ImageOwner<T> conversions
		template <typename T>
		static void convert_impl(ImageOwner<T>& out, const Script::ArrayType* inPtr)
		{
			out = Converter::toImageCopy<T>(const_cast<Script::ArrayType*>(inPtr));
		}


		template <typename T>
		static void convert_impl(ImageView<T>& out, const Script::ArrayType* inPtr)
		{
			out = Converter::toImage<T>(const_cast<Script::ArrayType*>(inPtr));
		}

		template <typename T>
		static void convert_impl(Script::ArrayType*& out, const ImageView<T>& in)
		{
			if ( out == nullptr )
				throw Converter::ImageConvertError("Output image data should already be created");
		}

	private:
		template <typename... TypeLayout, typename... Args, size_t... Is>
		static constexpr auto expand_parse_args_impl(std::tuple<TypeLayout...>, const std::tuple<Args...>& args, mph::index_sequence<Is...>) noexcept
			-> decltype(std::tuple_cat(ParserArg<TypeLayout>::argTuple(std::declval<Args>())...))
		{
			return std::tuple_cat(ParserArg<TypeLayout>::argTuple(std::get<Is>(args))...);
		}

		//////
		// expand_parse_args - Transform PyParser args so they can be passed to PyArg_ParseTuple
		template <typename TypeLayout, typename... Args>
		static constexpr auto expand_parse_args(const std::tuple<Args...>& args) noexcept
			-> decltype(expand_parse_args_impl(std::declval<TypeLayout>(), std::declval<std::tuple<Args...>>(), std::declval<mph::make_index_sequence<sizeof... (Args)>>()))
		{
			return expand_parse_args_impl(TypeLayout(), args, mph::make_index_sequence<sizeof... (Args)>());
		}
		

		// TODO: Change all these to compile-time string classes
		static std::string strcat_initializer(const std::initializer_list<const char*>& strsIn)
		{
			std::string out;
			out.reserve(2*strsIn.size() + 1);
			for ( const auto& it: strsIn )
				out += it;

			return out;
		}

		template <typename... TypeLayout>
		static std::string make_parse_str_impl(std::tuple<TypeLayout...>)
		{
			return strcat_initializer({ ParserArg<TypeLayout>::argString()... });
		}

		//////
		// make_parse_str - Create a PyArgs_ParseTuple format string from layout args
		template <typename TypeLayout>
		static std::string make_parse_str()
		{
			return make_parse_str_impl(TypeLayout());
		}


		template <typename... Args, std::size_t... Is>
		static bool parse_script_impl(PyObject* scriptIn, const std::string& format, const std::tuple<Args...>& args, mph::index_sequence<Is...>)
		{
			return (PyArg_ParseTuple(scriptIn, format.c_str(), std::get<Is>(args)...) != 0);
		}

		//////
		// parse_script - Unpack inopt argument reference tuple and pass along to PyArg_ParseTuple
		template <typename... Args>
		static bool parse_script(PyObject* scriptIn, const std::string& format, const std::tuple<Args...>& argpack)
		{
			return parse_script_impl(scriptIn, format, argpack, mph::make_index_sequence<sizeof... (Args)>());
		}
	};

};