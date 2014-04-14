#include "MexCommand.h"
#include "CudaProcessBuffer.cuh"
#include "Vec.h"

void MexOtsuThresholdFilter::execute( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] )
{
	int device = 0;

	if (nrhs>2)
		device = mat_to_c((int)mxGetScalar(prhs[2]));

	Vec<size_t> imageDims;
	HostPixelType* imageIn, * imageOut;
	CudaProcessBuffer cudaBuffer(device);
	setupImagePointers(prhs[0],&imageIn,&imageDims,&plhs[0],&imageOut);

	double alpha = 1.0;
	if (nrhs==2)
		alpha = mxGetScalar(prhs[1]);

	cudaBuffer.otsuThresholdFilter(imageIn,imageDims,alpha,&imageOut);
}

std::string MexOtsuThresholdFilter::check( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] )
{
	if (nrhs<1 || nrhs>3)
		return "Incorrect number of inputs!";

	if (nlhs!=1)
		return "Requires one output!";

	if (!mxIsUint8(prhs[0]))
		return "Image has to be formated as a uint8!";

	size_t numDims = mxGetNumberOfDimensions(prhs[0]);
	if (numDims>3 || numDims<2)
		return "Image can only be either 2D or 3D!";

	if (nrhs==2)
		if (!mxIsDouble(prhs[1]))
			return "Alpha needs to be a single double!";

	return "";
}

std::string MexOtsuThresholdFilter::printUsage()
{
	return "imageOut = CudaMex('OtsuThresholdFilter',imageIn,[alpha],[device]);";
}

std::string MexOtsuThresholdFilter::printHelp()
{
	std::string msg = "\tCalculates a two class threshold using Otsu's method.\n";
	msg += "\tEach pixel/voxel >= the threshold is set to the max value of the image space.";
	msg += "\tAll other values will be set at the minimum of the image space.\n";
	msg += "\n";
	return msg;
}

