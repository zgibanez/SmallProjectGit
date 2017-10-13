function d = DTWsample(U, V)

number_of_features = 4;
offset = 0;
fixed_length = true;

sigma = 5;
[M1,~] = size(U);
[M2,~] = size(V);
d(M1,M2) = 0;

if(fixed_length)
    [~,c1] = size(U(1,1:end));
    c2 = c1;
end

for i=1:M1
    for j=1:M2
        %If vectors do not have fixed length, they must be specified in
        %the second columns
        if(~fixed_length)
            c1 = U(i,2);
            c2 = V(j,2);
            UR = U(i,offset:offset+floor((c1-1)));
            VR = V(j,offset:offset+floor((c2-1)));
        else
            UR = U(i,1:c1);
            VR = V(j,1:c2);
        end
        UR = reshape(UR,number_of_features,floor(c1/number_of_features))';
        VR =reshape(VR,number_of_features,floor(c2/number_of_features))';

        d(i,j) = exp(-dtw(UR,VR)/(sigma*sigma));
    end
end
    
end
