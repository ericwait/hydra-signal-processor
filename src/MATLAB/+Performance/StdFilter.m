function [cTime,mTime,kernelName,c2Time] = StdFilter(im,nhood,numDevices)
    kernelName = 'StdFilter';
    
    cT = tic;
    imC = HIP.StdFilter(im,nhood,[],1);
    cTime = toc(cT);

    mT = tic;
    imM = HIP.Local.StdFilter(im,nhood,[],[],true);
    mTime = toc(mT);
    
    if (numDevices>1)
        c2T = tic;
        imC = HIP.StdFilter(im,nhood,[],[]);
        c2Time = toc(c2T);
    else
        c2Time = inf;
    end
end
