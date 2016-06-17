% MorphologicalOpening - imageOut = MorphologicalOpening(imageIn,kernel,device) 
function imageOut = MorphologicalOpening(imageIn,kernel,device)
    curPath = which('Cuda');
    curPath = fileparts(curPath);
    mutexfile = fullfile(curPath,sprintf('device%02d.txt',device));
    while(exist(mutexfile,'file'))
        pause(1);
    end
    f = fopen(mutexfile,'wt');
    fclose(f);

    [imageOut] = Cuda.Mex('MorphologicalOpening',imageIn,kernel,device);

    delete(mutexfile);
end
