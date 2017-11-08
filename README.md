# GeometricMedianADMM
An efficient multithread implmentation of ADMM for finding the geometric (euclidean) median of some points

The code takes in a matrix Z (dxN) which constists of N points which we want to take the geometric median of.
The algorithm also accepts a vector of non-negative w (Nx1) which can act as weights for the geoemtric median objective.
The objective is: min_x sum_{i=1}^N w_i || x - z_i || 

The code returns a vector x. In order to control the stopping criteria change the epsil. 
The code will stop when the ADMM residual goes below epsil or the number of iteration exceeds maxit.
There is no need for high accuracy as the optimal value is not very sensitive.

In order to change the number of threads change the defenition of NumThreads. 
If you don't want to use multithread implementation just remove the defention of MultiThread.
