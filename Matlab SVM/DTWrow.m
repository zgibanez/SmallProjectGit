function dtw_row = DTWrow(U,V)

offset = 2;
sigma = 3;

c1 = U(offset);
UR = U(3:3+floor((c1-1)));
UR = reshape(UR,4,floor(c1/4))';

[M,~] = size(V);
dtw_row(M) = 0;
 for i=1:M
    c2 = V(i,offset);
    VR = V(i,3:3+floor((c2-1)));
    VR =reshape(VR,4,floor(c2/4))';
    dtw_row(i) = exp(-dtw(UR,VR)/(sigma*sigma));
 end

end