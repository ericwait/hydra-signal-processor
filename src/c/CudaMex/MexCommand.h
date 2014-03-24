#pragma once
#include <map>
#include <string>
#include "mex.h"
#include "Vec.h"
#include "CudaProcessBuffer.cuh"

#define REGISTER_COMMAND(cmd) {MexCommand::addCommand(#cmd,(MexCommand*)new cmd());}

typedef unsigned char HostPixelType;

class MexCommand
{
public:
	virtual ~MexCommand();
	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) = 0;
	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) = 0;
	virtual std::string printUsage() = 0;
	virtual std::string printHelp() = 0;

	static void init();
	static bool needsInit(){return commandList.empty();}
	static MexCommand* getCommand(std::string cmd);
	static std::string printUsageList();
	static void cleanUp();
protected:
	MexCommand(){}
	static void addCommand(const std::string commandText, MexCommand* commandObject);
	void setupImagePointers( const mxArray* imageIn, HostPixelType** image, Vec<size_t>* dims, mxArray** argOut=NULL,
		HostPixelType** imageOut=NULL);

private:
	static std::map<std::string,MexCommand*> commandList;
};
 
 class AddConstant : MexCommand
 {
 public:
 	AddConstant(){}
 	virtual ~AddConstant(){}
 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string printUsage();
	virtual std::string printHelp();
 };
 
 class AddImageWith : MexCommand
 {
 public:
 	AddImageWith(){}
 	virtual ~AddImageWith(){}
 
 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string printUsage();
	virtual std::string printHelp();
 };
 
class ApplyPolyTransformation : MexCommand
{
public:
	ApplyPolyTransformation(){}
	virtual ~ApplyPolyTransformation(){}

	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string printUsage();
	virtual std::string printHelp();
};

// class CalculateMinMax : MexCommand
// {
// public:
// 	CalculateMinMax(){}
// 	virtual ~CalculateMinMax(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };

class ContrastEnhancement : MexCommand
{
public:
	ContrastEnhancement(){}
	virtual ~ContrastEnhancement(){}

	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string printUsage();
	virtual std::string printHelp();
};

 class GaussianFilter : MexCommand
 {
 public:
 	GaussianFilter(){}
 	virtual ~GaussianFilter(){}
 
 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string printUsage();
	virtual std::string printHelp();
 };

class Histogram : MexCommand
{
public:
	Histogram(){}
	virtual ~Histogram(){}

	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string printUsage();
virtual std::string printHelp();
};

// class Mask :MexCommand
// {
// public:
// 	Mask(){}
// 	virtual ~Mask(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
 //virtual std::string printHelp();
// };
// 
// class MaxFilterCircle : MexCommand
// {
// public:
// 	MaxFilterCircle(){}
// 	virtual ~MaxFilterCircle(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
// virtual std::string printHelp();
// };
// 
// class MaxFilterKernel : MexCommand
// {
// public:
// 	MaxFilterKernel(){}
// 	virtual ~MaxFilterKernel(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
// virtual std::string printHelp();
// };
// 
// class MaxFilterNeighborHood : MexCommand
// {
// public:
// 	MaxFilterNeighborHood(){}
// 	virtual ~MaxFilterNeighborHood(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
// virtual std::string printHelp();
// };
// 
// class MaximumIntensityProjection : MexCommand
// {
// public:
// 	MaximumIntensityProjection(){}
// 	virtual ~MaximumIntensityProjection(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
 //virtual std::string printHelp();
// };
// 
 class MeanFilter : MexCommand
 {
 public:
 	MeanFilter(){}
 	virtual ~MeanFilter(){}
 
 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string printUsage();
	virtual std::string printHelp();
 };

class MedianFilter : MexCommand
{
public:
	MedianFilter(){}
	virtual ~MedianFilter(){}

	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string printUsage();
	virtual std::string printHelp();
};

// class MinFilterCircle : MexCommand
// {
// public:
// 	MinFilterCircle(){}
// 	virtual ~MinFilterCircle(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class MinFilterKernel : MexCommand
// {
// public:
// 	MinFilterKernel(){}
// 	virtual ~MinFilterKernel(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class MinFilterNeighborhood : MexCommand
// {
// public:
// 	MinFilterNeighborhood(){}
// 	virtual ~MinFilterNeighborhood(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class MorphClosure : MexCommand
// {
// public:
// 	MorphClosure(){}
// 	virtual ~MorphClosure(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class MorphOpening : MexCommand
// {
// public:
// 	MorphOpening(){}
// 	virtual ~MorphOpening(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class MultiplyImage : MexCommand
// {
// public:
// 	MultiplyImage(){}
// 	virtual ~MultiplyImage(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class MultiplyImageWith : MexCommand
// {
// public:
// 	MultiplyImageWith(){}
// 	virtual ~MultiplyImageWith(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class NormalizedCovariance : MexCommand
// {
// public:
// 	NormalizedCovariance(){}
// 	virtual ~NormalizedCovariance(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };

class NormalizedHistogram : MexCommand
{
public:
	NormalizedHistogram(){}
	virtual ~NormalizedHistogram(){}

	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
	virtual std::string printUsage();
	virtual std::string printHelp();
};
 
// class OtsuThresholdFilter : MexCommand
// {
// public:
// 	OtsuThresholdFilter(){}
// 	virtual ~OtsuThresholdFilter(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class OtsuThesholdValue : MexCommand
// {
// public:
// 	OtsuThesholdValue(){}
// 	virtual ~OtsuThesholdValue(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class ImagePow : MexCommand
// {
// public:
// 	ImagePow(){}
// 	virtual ~ImagePow(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
// class SumArray : MexCommand
// {
// public:
// 	SumArray(){}
// 	virtual ~SumArray(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
//virtual std::string printHelp();
// };
// 
 class ReduceImage : MexCommand
 {
 public:
 	ReduceImage(){}
 	virtual ~ReduceImage(){}
 
 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
 	virtual std::string printUsage();
	virtual std::string printHelp();
 };
// 
// class ThresholdFilter : MexCommand
// {
// public:
// 	ThresholdFilter(){}
// 	virtual ~ThresholdFilter(){}
// 
// 	virtual void execute(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string check(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
// 	virtual std::string printUsage();
// virtual std::string printHelp();
// };
