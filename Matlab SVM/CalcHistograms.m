

clear;

txtDirectory = 'D:\SmallProject\txt_all\';
csvFiles = dir(strcat(txtDirectory,'\*.csv'));

for i=1:10
    fileData = readtable(strcat(txtDirectory, csvFiles(i).name));
    disp(csvFiles(i).name);
    idx = find(fileData.AU04_c);
    idx_not = find(~fileData.AU04_c);
    a = (fileData.eyeL_dist(idx)+fileData.eyeR_dist(idx))/2;
    b = (fileData.eyeL_dist(idx_not)+fileData.eyeR_dist(idx_not))/2;
    
    a1 = (fileData.eyeL_ang(idx)+fileData.eyeR_ang(idx))/2;
    b1 = (fileData.eyeL_ang(idx_not)+fileData.eyeR_ang(idx_not))/2;
    
    meandist(i,:) = [mean(a) mean(b) std(a) std(b)];
    meandistang(i,:) = [mean(a1) mean(b1) std(a1) std(b1)];
    correlations(i, :) = [corr(a1,fileData.AU04_r(idx)) corr(b1,fileData.AU04_r(idx_not))];
end