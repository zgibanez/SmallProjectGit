




%%DATA RETRIEVAL
clear;

kernel = 'linear';
%kernel = 'DTWsample';

%%Directory where all the features are extracted
txtDirectory = 'D:\SmallProject\txt_all\';

%%Choose the OutputDirectory
outputDirectory = 'D:\SmallProject\VideosOutput\annotations\';

%%File on which the annotations are made
[nameP,pathP] = uigetfile(strcat(outputDirectory,'*.csv'),'Select positive samples file');
[nameN,pathN] = uigetfile(strcat(outputDirectory,'*.csv'),'Select negative samples file');
annotations_positive = readtable(strcat(pathP,nameP), 'delimiter', ',', 'headerlines', 0, 'readvariablenames', true);
annotations_negative = readtable(strcat(pathN, nameN), 'delimiter', ',', 'headerlines', 0, 'readvariablenames', true);
names = [annotations_positive.file_name; annotations_negative.file_name];

%%Select features and number of frames
 
%%%SMILES%%%
%vars = {'AU06_r','AU07_r','AU12_r','AU25_r'};  %%Best feature set so far
%vars = {'AU01_r','AU02_r','AU04_r', 'AU12_r'}; %%Brow features
%vars = {'AU12_r', 'AU14_r', 'AU10_r', 'AU25_r'}; %%Lip features
%vars = {'AU06_r', 'AU10_r', 'AU12_r', 'AU14_r',  'AU25_r'}; %%Mix with lid tightener
%vars = {'AU06_r','AU07_r','AU10_r','AU12_r','AU25_r'};  %%Including lip
%stretching
%vars = {'AU12_r','AU25_r'}; %% Lips corner pull and lips part


%%%BROWS RAISE%%%
%vars = {'AU01_r','AU02_r','AU04_r','AU01_r_AU02_r'};
number_of_frames = 10;           %%Length in frames of the sample

%%CONCERN%%
vars = {'AU04_r','AU04_c','AU07_r','AU07_c', 'eyeL_ang_diff', 'eyeR_ang_diff'};  %%Brow lowerer (bad accuracy ~50%)
%vars = {'AU04_r','AU07_r','eyeL_ang_diff','eyeR_ang_diff'};  %%Brow


%%Collect all the samples
positive_samples = RetrieveFeatureData(annotations_positive,txtDirectory,vars,number_of_frames);
negative_samples = RetrieveFeatureData(annotations_negative,txtDirectory,vars,number_of_frames);
samples = [positive_samples; negative_samples];

%%Define the label vector
[Mp,~] = size(positive_samples);
[Mn,~] = size(negative_samples);
positive_labels(1:Mp)= 1;
negative_labels(1:Mn)=-1;
labels = [positive_labels negative_labels]';

%Train SVM with samples
initial_C = 0.01;

[SVM, perf, fp, fn] = TrainSVM(samples, labels, initial_C, 6, names, kernel);
