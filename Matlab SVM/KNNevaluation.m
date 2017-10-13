function predicted_label = KNNevaluation(train, labels, test, k, fixed_length)

    sigma = 5;
    [M1,~] = size(train);
    [M2,~] = size(test);
    d(M1,M2) = 0;
    
    if(fixed_length)
        [~,c1] = size(train(1,1:end));
        c2 = c1;
    end
    
    %% Calculate all pairwise DTW distances
    for i=1:M1
        for j=1:M2
            if(~fixed_length)
                c1 = train(i,2);
                c2 = test(j,2);
                UR = train(i,4:4+floor((c1-1)));
                UR = reshape(UR,4,floor(c1/4))';

                VR = test(j,4:4+floor((c2-1)));
                VR =reshape(VR,4,floor(c2/4))';
            else
                UR = train(i,1:c1);
                UR = reshape(UR,4,floor(c1/4))';
                
                VR = test(j,1:c2);
                VR =reshape(VR,4,floor(c2/4))';
            end
            
            d(i,j) = dtw(UR,VR);
        end
    end
    
    [M,N] = size(d);
    % D(i,j) = distance between training i and test j
    for i=1:N
        %Take the index of the minimum value elements
        for neighbours=1:k
            [val, idx] = min(d(:,i));
            distances(neighbours) = val;
            indices(neighbours) = idx;
            sample = train(1,:);
            matched_labels(neighbours) = labels(idx);
            d(idx,i) = Inf;
        end
        
        %%TODO: Use weighted average
        if(sum(matched_labels./distances)>=0)
            predicted_label(i) = 1;
        else
            predicted_label(i) = -1;
        end

    end

end