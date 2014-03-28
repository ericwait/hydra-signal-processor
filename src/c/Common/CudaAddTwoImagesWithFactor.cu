#include "CudaKernels.cuh"

__global__ void cudaAddTwoImagesWithFactor( CudaImageContainer imageIn1, CudaImageContainer imageIn2, CudaImageContainer imageOut, double factor,
										   DevicePixelType minValue, DevicePixelType maxValue )
{
	DeviceVec<size_t> coordinate;
	coordinate.x = threadIdx.x + blockIdx.x * blockDim.x;
	coordinate.y = threadIdx.y + blockIdx.y * blockDim.y;
	coordinate.z = threadIdx.z + blockIdx.z * blockDim.z;

	if (coordinate<imageIn1.getDeviceDims())
	{
		double additive = factor*(double)(imageIn2[coordinate]);
		double outValue = (double)(imageIn1[coordinate]) + additive;

		imageOut[coordinate] = (outValue>(double)maxValue) ? (maxValue) : ((outValue<(double)minValue) ? (minValue) : ((DevicePixelType)outValue));
	}
}

