#pragma once
#include "Vec.h"
#include "CudaDeviceStats.h"
#include "ImageContainer.h"


#ifdef IMAGE_PROCESSOR_DLL
#ifdef IMAGE_PROCESSOR_INTERNAL
#define IMAGE_PROCESSOR_API __declspec(dllexport)
#else
#define IMAGE_PROCESSOR_API __declspec(dllimport)
#endif // IMAGE_PROCESSOR_INTERNAL
#else
#define IMAGE_PROCESSOR_API
#endif // IMAGE_PROCESSOR_DLL

IMAGE_PROCESSOR_API void clearDevice();

IMAGE_PROCESSOR_API int deviceCount();
IMAGE_PROCESSOR_API int deviceStats(DevStats** stats);
IMAGE_PROCESSOR_API int memoryStats(size_t** stats);

IMAGE_PROCESSOR_API void maxFilter(const ImageContainer<bool> imageIn, ImageContainer<bool>& imageOut, Vec<size_t> kernelDims, float* kernel = NULL, int numIterations = 1, int device = -1);
IMAGE_PROCESSOR_API void maxFilter(const ImageContainer<char> imageIn, ImageContainer<char>& imageOut, Vec<size_t> kernelDims, float* kernel = NULL, int numIterations = 1, int device = -1);
IMAGE_PROCESSOR_API void maxFilter(const ImageContainer<unsigned char> imageIn, ImageContainer<unsigned char>& imageOut, Vec<size_t> kernelDims, float* kernel = NULL, int numIterations = 1, int device = -1);
IMAGE_PROCESSOR_API void maxFilter(const ImageContainer<short> imageIn, ImageContainer<short>& imageOut, Vec<size_t> kernelDims, float* kernel = NULL, int numIterations = 1, int device = -1);
IMAGE_PROCESSOR_API void maxFilter(const ImageContainer<unsigned short> imageIn, ImageContainer<unsigned short>& imageOut, Vec<size_t> kernelDims, float* kernel = NULL, int numIterations = 1, int device = -1);
IMAGE_PROCESSOR_API void maxFilter(const ImageContainer<int> imageIn, ImageContainer<int>& imageOut, Vec<size_t> kernelDims, float* kernel = NULL, int numIterations = 1, int device = -1);
IMAGE_PROCESSOR_API void maxFilter(const ImageContainer<unsigned int> imageIn, ImageContainer<unsigned int>& imageOut, Vec<size_t> kernelDims, float* kernel = NULL, int numIterations = 1, int device = -1);
IMAGE_PROCESSOR_API void maxFilter(const ImageContainer<float> imageIn, ImageContainer<float>& imageOut, Vec<size_t> kernelDims, float* kernel = NULL, int numIterations = 1, int device = -1);
IMAGE_PROCESSOR_API void maxFilter(const ImageContainer<double> imageIn, ImageContainer<double>& imageOut, Vec<size_t> kernelDims, float* kernel = NULL, int numIterations = 1, int device = -1);
