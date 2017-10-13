%%Get video classification frame by frame
outputDir = uigetdir;
outputDir = strcat(outputDir,'\');
%Classify videos frame by frame
offset = 30;
txtFiles = dir(strcat(txtDirectory,'*.csv'));

for i=1:numel(txtFiles)
    [~,name,~] = fileparts(txtFiles(i).name);
    path = strcat(outputDir,name,'.csv');
    
    %%If the video has not been proccessed, go for it
    if (exist(path,'file') == 0) 
        getVideoClassification(SVM,strcat(txtDirectory,txtFiles(i).name),vars,number_of_frames,offset,outputDir);
    end
end
