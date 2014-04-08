#include "CudaKernels.cuh"

__global__ void cudaMedianFilter( CudaImageContainer<DevicePixelType> imageIn, CudaImageContainer<DevicePixelType> imageOut,
								 Vec<size_t> hostKernelDims )
{
	extern __shared__ DevicePixelType vals[];
	DeviceVec<size_t> kernelDims = hostKernelDims;
	DeviceVec<size_t> coordinate;
	coordinate.x = threadIdx.x + blockIdx.x * blockDim.x;
	coordinate.y = threadIdx.y + blockIdx.y * blockDim.y;
	coordinate.z = threadIdx.z + blockIdx.z * blockDim.z;
	int offset = threadIdx.x + threadIdx.y*blockDim.x + threadIdx.z*blockDim.y*blockDim.x;
	offset *=  kernelDims.product();

	if (coordinate<imageIn.getDeviceDims())
	{
		int kernelVolume = 0;
		DeviceVec<size_t> kernelDims = hostKernelDims;
		DeviceVec<size_t> halfKernal = kernelDims/2;

		DeviceVec<size_t> curCoordIm = coordinate - halfKernal;
		curCoordIm.z = (coordinate.z<halfKernal.z) ? 0 : coordinate.z-halfKernal.z;
		for (; curCoordIm.z<=coordinate.z+halfKernal.z && curCoordIm.z<imageIn.getDeviceDims().z; ++curCoordIm.z)
		{
			curCoordIm.y = (coordinate.y<halfKernal.y) ? 0 : coordinate.y-halfKernal.y/2;
			for (; curCoordIm.y<=coordinate.y+halfKernal.y && curCoordIm.y<imageIn.getDeviceDims().y; ++curCoordIm.y)
			{
				curCoordIm.x = (coordinate.x<halfKernal.x) ? 0 : coordinate.x-halfKernal.x/2;
				for (; curCoordIm.x<=coordinate.x+halfKernal.x && curCoordIm.x<imageIn.getDeviceDims().x; ++curCoordIm.x)
				{
					vals[kernelVolume+offset] = imageIn[curCoordIm];
					++kernelVolume;
				}
			}
		}

		imageOut[coordinate] = cudaFindMedian(vals+offset,kernelVolume);
	}
	__syncthreads();
}

