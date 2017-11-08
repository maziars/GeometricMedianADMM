//
//  GeoMedianADMM.hpp
//  GeometricMedianADMM
//
//  Created by Maziar Sanjabi on 11/7/17.
//  Copyright © 2017 Maziar Sanjabi. All rights reserved.
//

#ifndef GeoMedianADMM_hpp
#define GeoMedianADMM_hpp

#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include <math.h>
#include "IO_MANAGER.hpp"





typedef struct algorithmStatistics_type
{   int it;
    double res;
    double obj;
    double time;
}algorithmStatistics_t;

#ifdef MultiThread

#include "barrier.hpp"
#include <pthread.h>




typedef struct __thread_args {
    int tid;      //thread id
    double* x;
    double* Z;
    double* w;
    double* Lambda;
    double* Y;
    double obj_i;
    double p_res_i;
    int N;
    int d;
    int it;
} thread_args;      //arguments for threads


extern pthread_barrier_t barrier;

void GeoMedianADMM_MultiThread(double* x, algorithmStatistics_t* stat, double* Z, double* w, int N, int d);

#endif //MultiThread



void printStat(char* desc, algorithmStatistics_t* stat);

void GeoMedianADMM_SingleThread(double* x, algorithmStatistics_t* stat, double* Z, double* w, int N, int d);




#endif /* GeoMedianADMM_hpp */
