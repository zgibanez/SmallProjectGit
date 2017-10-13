function trainResults = TrainKNN(samples, labels, numNeighbors)

    Mdl = fitknn(samples,labels, 'Distance', 'DTWsample', 'NumNeighbors', numNeighbors);

end