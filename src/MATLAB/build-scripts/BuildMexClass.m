function BuildMexClass(mexFile, packagePath, className, parentPackage)
    oldPath = pwd();
    cleanupObj = onCleanup(@()(cleanupFunc(oldPath)));

    [mexPath,mexName] = fileparts(mexFile);
    cd(mexPath);

    mexFunc = str2func(mexName);
    commandList = mexFunc('Info');

    % Create stable ordering of commands
    [~,srtidx] = sort({commandList.command}.');
    commandList = commandList(srtidx);

    cd(packagePath);
    
    % Delete old function definitions
    if ( exist(['@' className], 'dir') )
        delete(fullfile(['@' className], '*.m'));
    end
    
    makeClassdef(className, mexName, commandList);
    for i=1:length(commandList)
        makeStaticMethod(className, mexName, commandList(i), parentPackage);
    end

    copyfile(mexFile, ['@' className]);
end

function makeClassdef(className, mexName, commandList)
    if ( ~exist(['@' className],'dir') )
        mkdir(['@' className]);
    end
    classFile = fopen(fullfile(['@' className],[className '.m']), 'wt');

    fprintf(classFile, 'classdef (Abstract,Sealed) %s\n', className);

    fprintf(classFile, 'methods (Static)\n');
    for i=1:length(commandList)
        fprintf(classFile, '    %s\n', makePrototypeString(commandList(i)));
    end
    fprintf(classFile, 'end\n');

    fprintf(classFile, 'methods (Static, Access = private)\n');
    fprintf(classFile, '    varargout = %s(command, varargin)\n', mexName);
    fprintf(classFile, 'end\n');

    fprintf(classFile, 'end\n');

    fclose(classFile);
end

function makeStaticMethod(className, mexName, commandInfo, parentPackage)
    methodFile = fopen(fullfile(['@' className],[commandInfo.command '.m']), 'wt');

    helpLines = strsplit(commandInfo.help, '\n', 'CollapseDelimiters',false);
    validIdx = find(cellfun(@(x)(~isempty(x)), helpLines));
    
    validLines = helpLines(validIdx);
    
    % Autogenerated help has is organized so that the first two nonempy lines are:
    % 1. Internal command usage
    % 2. Command Summary
    % 3. ... Additional help lines
    
    % Ignore first helpline (internal usage string) which is replaced by prototypeString()
    summaryString = '';
    if ( length(validLines) >= 2 )
        % Second line is the 'command summary'
        summaryString = validLines{2};
    end

    % Write top line as <Command> - <Summary>, Summary may be empty
    fprintf(methodFile, '%% %s - %s\n', commandInfo.command, summaryString);

    % Write call protoype string
    fprintf(methodFile, '%%    %s\n', makePrototypeString(commandInfo,className,parentPackage,true));

    % Write remaining help lines directly
    if ( length(validIdx) > 2 )
        for i=validIdx(3):length(helpLines)
            fprintf(methodFile, '%%    %s\n', helpLines{i});
        end
    end

    % Output function body
    fprintf(methodFile, 'function %s\n', makePrototypeString(commandInfo));
    fprintf(methodFile, '    %s;\n', makeCommandString(className, mexName,commandInfo,parentPackage));
    fprintf(methodFile, 'end\n');

    fclose(methodFile);
end

function commandString = makeCommandString(className, mexName, commandInfo, parentPackage)
    commandString = '';
    if ( ~isempty(commandInfo.outArgs) )
         commandString = ['[' makeCommaList(commandInfo.outArgs) '] = '];
    end
    
    if ( ~exist('parentPackage','var') )
        parentPackage = [];
    else
        parentPackage = [parentPackage '.'];
    end

    mexCall = [parentPackage className '.' mexName];
    commandString = [commandString mexCall '(''' commandInfo.command ''''];
    if ( ~isempty(commandInfo.inArgs) )
         commandString = [commandString ',' makeCommaList(commandInfo.inArgs)];
    end
    commandString = [commandString ')'];
end

function protoString = makePrototypeString(commandInfo, className, parentPackage, leaveOptBrackets)
    if ( ~exist('className','var') )
        className = [];
    end
    if ( ~exist('parentPackage','var') )
        parentPackage = [];
    else
        parentPackage = [parentPackage '.'];
    end
    if ( ~exist('leaveOptBrackets','var') )
        leaveOptBrackets = false;
    end

    protoString = '';
    if ( ~isempty(commandInfo.outArgs) )
        protoString = makeCommaList(commandInfo.outArgs,leaveOptBrackets);

        if ( length(commandInfo.outArgs) > 1 )
            protoString = ['[' protoString ']'];
        end
        protoString = [protoString ' = '];
    end

    if ( ~isempty(className) )
        protoString = [protoString parentPackage className '.'];
    end

    protoString = [protoString commandInfo.command '('];
    if ( ~isempty(commandInfo.inArgs) )
         protoString = [protoString makeCommaList(commandInfo.inArgs, leaveOptBrackets)];
    end
    protoString = [protoString ')'];
end

function commaStr = makeCommaList(inStr, leaveOptBrackets)
    if (~exist('leaveOptBrackets','var') || isempty(leaveOptBrackets))
        leaveOptBrackets = false;
    end
    
    if ( leaveOptBrackets )
        commaStr = inStr;
    else
        commaStr = regexprep(inStr, '\[(\w+)\]', '$1');
    end
end

function cleanupFunc(oldPath)
    cd(oldPath);
end
