function K = rbfKernel(U,V)
   sigma = 0.2087;
   gamma = 1 ./ (2*(sigma ^2));
   K = exp(-gamma .* pdist2(U,V,'euclidean').^2);
end