% EntropyFilter - This calculates the entropy within the neighborhood given by the kernel.
%    [imageOut] = HIP.EntropyFilter(imageIn,kernel,[device])
%    	imageIn = This is a one to five dimensional array. The first three dimensions are treated as spatial.
%    		The spatial dimensions will have the kernel applied. The last two dimensions will determine
%    		how to stride or jump to the next spatial block.
%    
%    	kernel = This is a one to three dimensional array that will be used to determine neighborhood operations.
%    		In this case, the positions in the kernel that do not equal zeros will be evaluated.
%    		In other words, this can be viewed as a structuring element for the max neighborhood.
%    
%    	device (optional) = Use this if you have multiple devices and want to select one explicitly.
%    		Setting this to [] allows the algorithm to either pick the best device and/or will try to split
%    		the data across multiple devices.
%    
%    	imageOut = This will be an array of the same type and shape as the input array.
%    

function [imageOut] = EntropyFilter(imageIn,kernel,device)
    try
        [imageOut] = HIP.Cuda.EntropyFilter(imageIn,kernel,device);
    catch errMsg
        warning(errMsg.message);
        [imageOut] = HIP.Local.EntropyFilter(imageIn,kernel,device);
    end
end
