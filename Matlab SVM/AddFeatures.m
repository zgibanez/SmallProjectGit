%%ADD FEATURES
clear;

txtDirectory = 'D:\SmallProject\txt_all\';
csvFiles = dir(strcat(txtDirectory,'\*.csv'));
txtFiles = dir(strcat(txtDirectory,'\*.txt'));

% %%AU01+AU02
% for i=1:numel(csvFiles)
%   [~,fileName,~] = fileparts(csvFiles(i).name); 
%   path = strcat(txtDirectory,fileName,'.csv');
%   
%   disp(strcat('Writing AU01_r + AU02_r column in file ',int2str(i),' of ', int2str(numel(csvFiles))));
% 
%   fileData = readtable(strcat(txtDirectory, csvFiles(i).name));
%   fileDatatxt = readtable(strcat(txtDirectory, txtFiles(i).name));
%   
%   file_corrupted = false;
%   if(height(fileDatatxt) > height(fileData))
%         file_corrupted = true;
%   end
% 
%   if(file_corrupted)
%     disp(strcat('File_ ',csvFiles(i).name, 'was corrupted. File will be rewritten.'));
%     fileDatatxt.AU01_r_AU02_r = fileDatatxt.AU01_r + fileDatatxt.AU02_r;
%     writetable(fileDatatxt,path);
%   else
%     Exist_Column = strcmp('AU01_r_AU02_r',fileData.Properties.VariableNames);
%     val = Exist_Column(Exist_Column==1) ;
%     if(val==1)
%           disp(' File already has column calculated.');
%           continue;
%     end
%     fileData.AU01_r_AU02_r = fileData.AU01_r + fileData.AU02_r;
%     writetable(fileData,path);
%   end
% 
% end
% 
% 
% for i=1:numel(csvFiles)
%   [~,fileName,~] = fileparts(csvFiles(i).name); 
%   path = strcat(txtDirectory,fileName,'.csv');
%   
%   
%   
%   disp(strcat('Writing eye aperture distance in file ',int2str(i),' of ', int2str(numel(csvFiles))));
%   
%   fileData = readtable(strcat(txtDirectory, csvFiles(i).name));
%   Exist_Column = strcmp('eyeL_dist',fileData.Properties.VariableNames);
%   val = Exist_Column(Exist_Column==1) ;
%      if (val==1)
%          disp(' File already has column calculated.');
%          continue;
%      end
% 
%   cR_R = sqrt((fileData.X_37 - fileData.X_41).^2+ (fileData.Y_37 - fileData.Y_41).^2 + (fileData.Z_37 - fileData.Z_41).^2);
%   cR_L = sqrt((fileData.X_38 - fileData.X_40).^2+ (fileData.Y_38 - fileData.Y_40).^2 + (fileData.Z_38 - fileData.Z_40).^2);
%   cL_R = sqrt((fileData.X_43 - fileData.X_47).^2+ (fileData.Y_43 - fileData.Y_47).^2 + (fileData.Z_43 - fileData.Z_47).^2);
%   cL_L = sqrt((fileData.X_46 - fileData.X_44).^2+ (fileData.Y_46 - fileData.Y_44).^2 + (fileData.Z_46 - fileData.Z_44).^2);
% 
%   fileData.eyeL_dist = (cR_R+cR_L)./2;
%   fileData.eyeR_dist = (cL_R+cL_L)./2;
%   writetable(fileData,path);
% end
% 
% 
% %%EYE APERTURE ANGLE
% for i=1:numel(csvFiles)
%   [~,fileName,~] = fileparts(csvFiles(i).name); 
%   path = strcat(txtDirectory,fileName,'.csv');
% 
%   disp(strcat('Opening file ',int2str(i),' of ', int2str(numel(csvFiles))));
%   fileData = readtable(strcat(txtDirectory, csvFiles(i).name));
%   Exist_Column = strcmp('eyeL_dist',fileData.Properties.VariableNames);
%   Exist_Column_angle = strcmp('eyeL_ang',fileData.Properties.VariableNames);
%   val1 = Exist_Column(Exist_Column==1) ;
%   val2 = Exist_Column_angle(Exist_Column_angle==1) ;
%  if (or(val==1,val2 ==1))
%      disp(' File already has column calculated.');
%      continue;
%  end
%   
%   
%   aR_R = sqrt((fileData.X_36 - fileData.X_41).^2+ (fileData.Y_36 - fileData.Y_41).^2 + (fileData.Z_36 - fileData.Z_41).^2);
%   bR_R = sqrt((fileData.X_36 - fileData.X_37).^2+ (fileData.Y_36 - fileData.Y_37).^2 + (fileData.Z_36 - fileData.Z_37).^2);
%   cR_R = sqrt((fileData.X_37 - fileData.X_41).^2+ (fileData.Y_37 - fileData.Y_41).^2 + (fileData.Z_37 - fileData.Z_41).^2);
%   cos_alpha_R_R = (cR_R.^2 - aR_R.^2 - bR_R.^2)./(-2*aR_R.*bR_R);
%   alpha_R_R = acos(cos_alpha_R_R);
%   
%   aR_L = sqrt((fileData.X_38 - fileData.X_39).^2+ (fileData.Y_38 - fileData.Y_39).^2 + (fileData.Z_38 - fileData.Z_39).^2);
%   bR_L = sqrt((fileData.X_40 - fileData.X_39).^2+ (fileData.Y_40 - fileData.Y_39).^2 + (fileData.Z_40 - fileData.Z_39).^2);
%   cR_L = sqrt((fileData.X_38 - fileData.X_40).^2+ (fileData.Y_38 - fileData.Y_40).^2 + (fileData.Z_38 - fileData.Z_40).^2);
%   cos_alpha_R_L = (cR_L.^2 - aR_L.^2 - bR_L.^2)./(-2*aR_L.*bR_L);
%   alpha_R_L = acos(cos_alpha_R_L);
% 
%   aL_R = sqrt((fileData.X_43 - fileData.X_42).^2+ (fileData.Y_43 - fileData.Y_42).^2 + (fileData.Z_43 - fileData.Z_42).^2);
%   bL_R = sqrt((fileData.X_47 - fileData.X_42).^2+ (fileData.Y_47 - fileData.Y_42).^2 + (fileData.Z_47 - fileData.Z_42).^2);
%   cL_R = sqrt((fileData.X_43 - fileData.X_47).^2+ (fileData.Y_43 - fileData.Y_47).^2 + (fileData.Z_43 - fileData.Z_47).^2);
%   cos_alpha_L_R = (cL_R.^2 - aL_R.^2 - bL_R.^2)./(-2*aL_R.*bL_R);
%   alpha_L_R = acos(cos_alpha_L_R);
%   
%   aL_L = sqrt((fileData.X_45 - fileData.X_44).^2+ (fileData.Y_45 - fileData.Y_44).^2 + (fileData.Z_45 - fileData.Z_44).^2);
%   bL_L = sqrt((fileData.X_45 - fileData.X_46).^2+ (fileData.Y_45 - fileData.Y_46).^2 + (fileData.Z_45 - fileData.Z_46).^2);
%   cL_L = sqrt((fileData.X_46 - fileData.X_44).^2+ (fileData.Y_46 - fileData.Y_44).^2 + (fileData.Z_46 - fileData.Z_44).^2);
%   cos_alpha_L_L = (cL_L.^2 - aL_L.^2 - bL_L.^2)./(-2*aL_L.*bL_L);
%   alpha_L_L = acos(cos_alpha_L_L);
%   
%   fileData.eyeL_ang = (alpha_R_R);
%   fileData.eyeL_ang = fileData.eyeL_ang*(180/pi);
%   fileData.eyeR_ang = (alpha_L_L);
%   fileData.eyeR_ang = fileData.eyeR_ang*(180/pi);
%   
%   writetable(fileData,path);
%   
% end

%%NORMALIZED ANGLE DIFFERENCES
for i=1:numel(csvFiles)
  [~,fileName,~] = fileparts(csvFiles(i).name); 
  path = strcat(txtDirectory,fileName,'.csv');
  fileData = readtable(strcat(txtDirectory, csvFiles(i).name)); 
  
  disp(strcat('Opening file ',int2str(i),' of ', int2str(numel(csvFiles))));

  norm_eyeL = (fileData.eyeL_ang-min(fileData.eyeL_ang))./ (max(fileData.eyeL_ang)-min(fileData.eyeL_ang));
  norm_eyeR = (fileData.eyeR_ang-min(fileData.eyeR_ang))./ (max(fileData.eyeR_ang)-min(fileData.eyeR_ang));
     
  eyeL_ang_diff = [0; norm_eyeL(2:end,:)-norm_eyeL(1:(end-1),:)];
  fileData.eyeL_ang_diff = eyeL_ang_diff;
  
  eyeR_ang_diff = [0; norm_eyeR(2:end,:)-norm_eyeR(1:(end-1),:)];
  fileData.eyeR_ang_diff = eyeR_ang_diff;
  
  writetable(fileData,path);
  
end
