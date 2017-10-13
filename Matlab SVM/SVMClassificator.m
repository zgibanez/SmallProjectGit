
clear all;

features_different_lenghts = false;
perform_cross_validation = false;

%%% TRAINING DATA %%%

% Collect positive samples
positive_table = readtable('D:\SmallProject\VideosOutput\annotations\smile20.csv', 'delimiter', ',', 'headerlines', 1, 'readvariablenames', false);
positive_samples = table2array(positive_table(1:end,4:end));
[Mp,Np] = size(positive_samples);

% Collect negative samples
negative_table = readtable('D:\SmallProject\VideosOutput\annotations\nsmile20.csv', 'delimiter', ',', 'headerlines', 1, 'readvariablenames', false);
negative_samples = table2array(negative_table(1:end,4:end));
[Mn,Nn] = size(negative_samples);

% Define label vector
positive_labels(1:Mp) = 1;
negative_labels(1:Mn) = -1;
labels = [positive_labels negative_labels]';

%If the matrices have different dimensions, fill the smaller with zeros
%until they match
    max_col= 0;
if(features_different_lenghts)
    if(Nn > Np)
        max_col = Nn;
        positive_samples(1,Nn) = 0;
    else
        max_col = Np;
        negative_samples(1,Np) = 0;
    end
end
samples = [positive_samples; negative_samples];
[Ms,Ns] = size(samples);

%%% TEST DATA %%%
% test_positive_samples = csvread('D:\SmallProject\VideosOutput\annotations\test_smile.csv');
% test_negative_samples = csvread('D:\SmallProject\VideosOutput\annotations\test_nsmile.csv');
% [Mtp,Ntp] = size(test_positive_samples);
% [Mtn,Ntn] = size(test_negative_samples);
% 
% test_negative_samples(1,max_col) = 0;
% test_positive_samples(1,max_col) = 0;
% 
% test_samples = [test_positive_samples; test_negative_samples];
% [Ms,Ns] = size(test_samples);
% test_labels(1:Mtp)=1;
% test_labels((Mtp+1):(Mtp+Mtn))=-1;

%% Cross validation %%
if(perform_cross_validation)
    kfolds = 10;
    cv = cvpartition(labels,'k', 10);
    err = zeros(1,cv.NumTestSets);
    for i=1:cv.NumTestSets
        trIdx = cv.training(i);
        tsIdx = cv.test(i);
        tsVector = labels(tsIdx);
        predicted_labels = KNNevaluation(samples(trIdx,:),labels(trIdx,:),samples(tsIdx,:),3, true);
        
        %% Measure error
        for u=1:length(predicted_labels)
            if (~isequal(predicted_labels(u),tsVector(u)))
                err(i) = err(i)+1;
            end
        end
    end
    error = sum(err)/sum(cv.TestSize);
end




%%% SVM TRAINING %%%

%%% KNN TRAINING %%%
%dtwDist = @(U,V)DTWrow(U,V);
%[ind, D] = knnsearch(samples, test_samples(1:10,:),'Distance',dtwDist,'k',5);
%Mdl = fitcknn(samples,labels, 'Distance', dtwDist, 'NumNeighbors', 5);





%Measure generalization error
%predict(Mdl,samples(1:10,:));