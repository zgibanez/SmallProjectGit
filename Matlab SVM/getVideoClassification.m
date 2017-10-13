function getVideoClassification(MdlSVM,txtFile,vars,number_of_frames, offset, outputDir)


[~,fileName,ext] = fileparts(txtFile)

fileData = readtable(txtFile);
[M,N] = size(fileData);
prediction(1:M,1) = -1;
distance2boundary(1:M,:)= -1;
sv = MdlSVM.SupportVectors;
alphaHat = MdlSVM.Alpha;
bias = MdlSVM.Bias;

for i=offset:1:(M-offset)
    instance = table2array(fileData(i-(number_of_frames-1):i,vars));
    instance = reshape(instance',length(vars)*number_of_frames,1);
    p = predict(MdlSVM,instance');
    prediction(i) = p;
    distance2boundary(i) = DTWsample(sv,instance')'*alphaHat(:) + bias;
end

 csvwrite(strcat(outputDir,fileName,'.csv'),[prediction distance2boundary]);
end