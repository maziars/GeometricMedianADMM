//
//  GeoMedianADMM.cpp
//  GeometricMedianADMM
//
//  Created by Maziar Sanjabi on 11/7/17.
//  Copyright © 2017 Maziar Sanjabi. All rights reserved.
//

#include "GeoMedianADMM.hpp"



#ifdef MultiThread
pthread_t thread[NumThreads];
pthread_barrier_t barrier;
thread_args targs[NumThreads];
double res_global = 0.0;
double obj_global = 0.0;
#endif//MultiThread


void printStat(char* desc, algorithmStatistics_t* stat)
{
    printf("\n%s", desc);
    printf("\n---\n");
    printf("iterations number: %d, \n", stat->it);
    printf("final residual: %f, \n", stat->res);
    printf("final objective: %f, \n", stat->obj);
    printf("elapsed time: %f, \n", stat->time);
    printf("\n---\n");
}



double Fro(double* a, int n)
{
    double S = 0;
    for(int i=0; i<n; i++)
        S = S + a[i]*a[i];
    return sqrt(S);
}


void mean(double* m, double* M, int N, int d)
{
    // make sure that entries in m are zeroed beforehand
    for(int i=0;i<N; i++)
    {
        int base = d*i;
        for(int j=0;j<d;j++)
        {
            m[j] = m[j] + M[base+j];
        }
    }
    
    for(int j=0;j<d;j++)
    {
        m[j] = m[j]/N;
    }
}


double UpdateY(double* Y, double* Lambda, double* x, double* Z, double* w, int N, int d, int i)
{
    double p_res_i = 0;
    int base = i*d;
    double* V = (double*) malloc(sizeof(double)*d);
    double size_V = 0;
    for(int j=base; j<base+d; j++)
    {
        V[j-base] = rho*(x[j-base] - Z[j]) + Lambda[j];
        size_V = size_V + V[j-base]*V[j-base];
    }
    size_V = sqrt(size_V);
    double delta = (size_V-w[i])/rho;
    if(delta>0)
    {
        delta = 1.0/(w[i]/delta + rho);
    }
    else
    {
        delta = 0.0;
    }
    
    for(int j=base; j<base+d; j++)
    {
        double Yold = Y[j];
        Y[j] = V[j-base]*delta;
        p_res_i = p_res_i + (Y[j]-Yold)*(Y[j]-Yold);
    }
    
    
    free(V);
    return p_res_i;
}


double UpdateX(double* Y, double* Lambda, double* x, double* Z, double* w, int N, int d)
{
    double* z = (double*) calloc(d, sizeof(double));
    double* y = (double*) calloc(d, sizeof(double));
    double* lambda = (double*) calloc(d, sizeof(double));
    
    mean(z, Z , N, d);
    mean(y, Y , N, d);
    mean(lambda, Lambda , N, d);
    
    double temp = 1.0/rho;
    for(int i=0; i<d; i++){
        x[i] = z[i] + y[i] - temp*lambda[i];
    }
    free(z);
    free(y);
    free(lambda);
    
    double d_res = 0.0;
    for(int j=0;j<N;j++)
    {
        int base = j*d;
        for(int i=base; i<base+d; i++)
        {
            d_res = d_res + (x[i-base]-Z[i]-Y[i])*(x[i-base]-Z[i]-Y[i]);
        }
    }
    d_res = d_res/rho;
    return d_res;
}


double obj_i(double*x, double* Z, double* w, int N, int d, int i)
{
    double S = 0;
    int base = i*d;
    for(int j=base;j<base+d; j++)
        S = S + (x[j-base]-Z[j])*(x[j-base]-Z[j]);
    
    S = w[i]*sqrt(S);
    return S;
}



void UpdateLambda(double* Y, double* Lambda, double* x, double* Z, double* w, int N, int d, int i)
{
    int base = i*d;
    for(int j=base; j<base+d; j++)
        Lambda[j] = Lambda[j] + rho*(x[j-base]-Z[j]-Y[j]);
}

void GeoMedianADMM_SingleThread(double* x, algorithmStatistics_t* stat, double* Z, double* w, int N, int d){
    
    // initialize
    double* Lambda = (double*) calloc(N*d, sizeof(double));
    double* Y      = (double*) calloc(N*d, sizeof(double));
    for(int i=0; i<d; i++)
        x[i] = 0.0;
    
    int it;
    double obj = 0;
    double res = 0;
    for(it=0; it<maxit; it++)
    {
        // update Y
        double p_res = 0.0;
        for(int i=0; i<N; i++)
        {
            p_res = p_res + UpdateY(Y, Lambda, x, Z, w, N, d, i);
        }
        p_res = p_res*rho;
        
        //show_row_rect_matrix("Y' is:", Y, N, d);
        
        // update x
        double d_res = UpdateX(Y, Lambda, x, Z, w, N, d);
        
        //show_row_rect_matrix("x is:", x, d, 1);
        
        // update Lambda & Compute objective
        obj = 0.0;
        for(int i=0; i<N; i++)
        {
            UpdateLambda(Y, Lambda, x, Z, w, N, d, i);
            obj = obj + obj_i(x, Z, w, N, d, i);
        }
        
        //show_row_rect_matrix("Lambda' is:", Lambda, N, d);
        
        res = p_res + d_res;
        
        //if((it%10)==0)
            //printf("iteration number: %d, residual: %1.5f, objective: %1.1f\n", it, res, obj);
        
        if(res<epsil)
        {
            //printf("iteration number: %d, residual: %1.5f, objective: %1.1f\n", it, res, obj);
            break;
        }
        
        
    }
    stat[0].it = it;
    stat[0].obj = obj;
    stat[0].res = res;
    
    free(Lambda);
    free(Y);
    
}















//---------------------------------------------------------------
#ifdef MultiThread

void* ADMM_Threads(void *args)
{
    // parsing the input
    thread_args *largs = (thread_args *)args;
    int tid = largs->tid;
    double* x = largs->x;
    double* Z = largs->Z;
    double* w = largs->w;
    double* Lambda = largs->Lambda;
    double* Y = largs->Y;
    int N = largs->N;
    int d = largs->d;
    
    int base = tid*int(N/NumThreads);
    int end;
    if((tid+1)== NumThreads){
        end = N;
    }
    else{
        end = (tid+1)*int(N/NumThreads);
    }
    
    for(int it=0; it<maxit;it++)
    {
        largs->it = it;
        // update Y
        double p_res = 0.0;
        for(int i=base; i<end; i++)
        {
            p_res = p_res + UpdateY(Y, Lambda, x, Z, w, N, d, i);
        }
        p_res = p_res*rho;
        largs->p_res_i = p_res;
        
        // Wait for everybody to reach here !
        pthread_barrier_wait(&barrier);
        
        
        if(tid == 0)// Only one of them updates x
        {
            
            double d_res = UpdateX(Y, Lambda, x, Z, w, N, d);
            double p_res = 0.0;
            for(int j=0; j<NumThreads; j++){
                //Check the threads to make sure they are all on the same iteration
                if(targs[j].it != it)
                    printf("Asynchrony on thread: %d", j);
                p_res = p_res + targs[j].p_res_i;
            }
            res_global = p_res + d_res;
        }
        pthread_barrier_wait(&barrier);
        
        if(res_global<epsil)
            break;
        
        for(int i=base; i<end; i++)
        {
            UpdateLambda(Y, Lambda, x, Z, w, N, d, i);
        }
        
        
        
        
    }
    double obj_l = 0;
    for(int i=base; i<end; i++)
    {
        obj_l = obj_l + obj_i(x, Z, w, N, d, i);
    }
    largs->obj_i = obj_l;
    pthread_barrier_wait(&barrier);
    
    
    if(tid == 0){
        obj_global = 0;
        for(int j=0; j<NumThreads;j++){
            //Check the threads to make sure they are all on the same iteration
            if(targs[j].it != largs->it)
                printf("Asynchrony on thread: %d", j);
            obj_global = obj_global + targs[j].obj_i;
        }
    }
    
    return NULL;
}



void GeoMedianADMM_MultiThread(double* x, algorithmStatistics_t* stat, double* Z, double* w, int N, int d){
    
    // initialize
    double* Lambda = (double*) calloc(N*d, sizeof(double));
    double* Y      = (double*) calloc(N*d, sizeof(double));
    
    
    pthread_barrier_init(&barrier, NULL, NumThreads);
    
    for(int i = 0; i < NumThreads; i++) {
        targs[i].tid = i;
        targs[i].x = x;
        targs[i].Z = Z;
        targs[i].N = N;
        targs[i].d = d;
        targs[i].Y = Y;
        targs[i].Lambda = Lambda;
        targs[i].w = w;
        pthread_create(&thread[i], NULL, ADMM_Threads, &targs[i]);
    }
    
    for(int i = 0; i < NumThreads; i++) {
        pthread_join(thread[i], NULL);
    }
    
    
    
    stat[0].it = targs[0].it;
    stat[0].obj = obj_global;
    stat[0].res = res_global;
    
    free(Lambda);
    free(Y);
    pthread_barrier_destroy(&barrier);
    
}

#endif // MultiThread
