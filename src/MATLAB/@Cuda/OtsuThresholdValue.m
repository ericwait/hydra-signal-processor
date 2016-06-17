% OtsuThresholdValue - threshold = OtsuThresholdValue(imageIn,device) 
function threshold = OtsuThresholdValue(imageIn,device)
    curPath = which('Cuda');
    curPath = fileparts(curPath);
    mutexfile = fullfile(curPath,sprintf('device%02d.txt',device));
    while(exist(mutexfile,'file'))
        pause(1);
    end
    f = fopen(mutexfile,'wt');
    fclose(f);

    [threshold] = Cuda.Mex('OtsuThresholdValue',imageIn,device);

    delete(mutexfile);
end