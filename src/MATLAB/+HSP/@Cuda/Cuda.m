classdef (Abstract,Sealed) Cuda
methods (Static)
    commandInfo = Info()
    Help(command)
    [numCudaDevices,memoryStats] = DeviceCount()
    deviceStatsArray = DeviceStats()
    arrayOut = Closure(arrayIn,kernel,numIterations,device)
    arrayOut = ElementWiseDifference(array1In,array2In,device)
    arrayOut = EntropyFilter(arrayIn,kernel,device)
    arrayOut = Gaussian(arrayIn,sigmas,numIterations,device)
    [minValue,maxValue] = GetMinMax(arrayIn,device)
    arrayOut = HighPassFilter(arrayIn,sigmas,device)
    arrayOut = LoG(arrayIn,sigmas,device)
    arrayOut = MaxFilter(arrayIn,kernel,numIterations,device)
    arrayOut = MeanFilter(arrayIn,kernel,numIterations,device)
    arrayOut = MedianFilter(arrayIn,kernel,numIterations,device)
    arrayOut = MinFilter(arrayIn,kernel,numIterations,device)
    [minOut,maxOut] = MinMax(arrayIn,device)
    arrayOut = MultiplySum(arrayIn,kernel,numIterations,device)
    arrayOut = Opener(arrayIn,kernel,numIterations,device)
    arrayOut = StdFilter(arrayIn,kernel,numIterations,device)
    valueOut = Sum(arrayIn,device)
    arrayOut = VarFilter(arrayIn,kernel,numIterations,device)
    arrayOut = WienerFilter(arrayIn,kernel,noiseVariance,device)
end
methods (Static, Access = private)
    varargout = Mex(command, varargin)
end
end
