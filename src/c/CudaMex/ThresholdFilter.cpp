#include "MexCommand.h"
#include "CudaProcessBuffer.cuh"

void ThresholdFilter::execute( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] )
{
	Vec<size_t> imageDims;
	HostPixelType* imageIn, * imageOut;
	CudaProcessBuffer cudaBuffer;
	setupImagePointers(prhs[0],&imageIn,&imageDims,&plhs[0],&imageOut);

	double thresh = mxGetScalar(prhs[1]);

	cudaBuffer.thresholdFilter(imageIn,imageDims,(DevicePixelType)thresh,&imageOut);
}

std::string ThresholdFilter::check( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] )
{
	if (nrhs!=2)
		return "Incorrect number of inputs!";

	if (nlhs!=1)
		return "Requires one output!";

	if (!mxIsUint8(prhs[0]))
		return "Image has to be formated as a uint8!";

	size_t numDims = mxGetNumberOfDimensions(prhs[0]);
	if (numDims>3 || numDims<2)
		return "Image can only be either 2D or 3D!";

	if (!mxIsDouble(prhs[1]))
		return "Threshold needs to be a single double!";

	return "";
}

std::string ThresholdFilter::printUsage()
{
	return "imageOut = CudaMex('ThresholdFilter',imageIn,threshold)";
}

std::string ThresholdFilter::printHelp()
{
	std::string msg = "\tMaps any value >= thresh to the max value of the image space.";
	msg += "\tAll other values will be set at the minimum of the image space.\n";
	msg += "\n";
	return msg;
}
