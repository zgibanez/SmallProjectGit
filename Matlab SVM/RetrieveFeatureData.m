 function sampleTable = RetrieveFeatureData(annotations,txtDirectory,vars, number_of_frames)

%annotations : CSV file where the frame of the instance is written on the
%second columns
%txtDirectory : directory of the txtFiles
%vars : variables chosen
%number_of_frames : number of frames to take before the capture

csvFiles = dir(strcat(txtDirectory,'*.csv'));
sampleTable = [];

for i=1:numel(csvFiles)
    
    [~,fileName,~] = fileparts(csvFiles(i).name); 
    idx = strcmp(annotations.file_name,fileName);
    
    %sum(idx) will be >0 if there were annotations with the fileName 
    if(sum(idx)>0)
        startFrames = annotations(idx,:).start_frame;
        [M,~] = size(startFrames);
        fileData = readtable(strcat(txtDirectory, csvFiles(i).name));
        for j=1:M
            instance = table2array(fileData(startFrames(j)-(number_of_frames-1):startFrames(j),vars));
            instance = reshape(instance',length(vars)*number_of_frames,1);
            sampleTable = [sampleTable; instance'];
        end
    else
        continue;
    end 
    
end


end