function [Mdl, trainResults, fp_names, fn_names] = TrainSVM(samples, labels, initial_C, orders_of_magnitude, names, kernel)

cross_val = false;
kfolds = 5;
PR = [];
fp_names = [];
fn_names = [];
%dtwDist = @(U,V)DTWsample(U,V,4,0,true);
        %Mdl = fitcsvm(samples,labels','KernelFunction','DTWsample');

%         if(cross_val)
%             %Cross validation
%             CVSVMModel = crossval(Mdl);
%             perf(i) = kfoldLoss(CVSVMModel);
%         end
%Get a test data and a sample data

for i=1:orders_of_magnitude

    true_positives = 0;
    true_negatives = 0;
    false_positives = 0;
    false_negatives = 0;

    C(i) = initial_C*(10^i);

    %%Divide data into k folds
    cv = cvpartition(labels,'k', 10);
    
    for j=1:cv.NumTestSets
        
        %Grab the j-th test and training set
        training = samples(cv.training(j),:);
        training_labels = labels(cv.training(j));
        test_samples = samples(cv.test(j),:);
        test_labels = labels(cv.test(j));
        test_names = names(cv.test(j));
        
        %Train the SVM with the training data and predict the test samples
        Mdl = fitcsvm(training,training_labels','KernelFunction',kernel,'BoxConstraint',C(i));
        predicted_label = predict(Mdl,test_samples);
        
        %Count performance
        [Ms,~] = size(predicted_label);
        for u=1:Ms
            if(test_labels(u)==1 && predicted_label(u)==1)
                true_positives = true_positives+1;
            end
            if(test_labels(u)==-1 && predicted_label(u)==-1)
                true_negatives = true_negatives+1;
            end
            if(test_labels(u)==-1 && predicted_label(u)==1)
                false_positives = false_positives+1;
                fp_names = [fp_names; test_names(u)];
            end
            if(test_labels(u)==1 && predicted_label(u)==-1)
                false_negatives = false_negatives+1;
                fn_names = [fn_names; test_names(u)];
            end
        end
        precision(j) = true_positives / (false_positives+true_positives);
        recall(j) = true_positives / (false_negatives+true_positives);
    end
    
    F1score = 2*mean(precision)*mean(recall)/(mean(precision)+mean(recall));
    PR(i,:) = [mean(precision) mean(recall) F1score];
    
end

%Train with the parameters that achieved the best PR score
[~,idx] = max(PR(:,3));
bestC = C(idx);
Mdl = fitcsvm(training,training_labels','KernelFunction','DTWsample','BoxConstraint',bestC);
trainResults = [C' PR];

end
