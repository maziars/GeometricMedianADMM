//
//  main.cpp
//  GeometricMedianADMM
//
//  Created by Maziar Sanjabi on 11/6/17.
//  Copyright © 2017 Maziar Sanjabi. All rights reserved.
//

#include <iostream>
#include "IO_MANAGER.hpp"
#include "Constants.h"
#include <ctime>
#include "GeoMedianADMM.hpp"





int main(int argc, const char * argv[]) {
    // insert code here...
    
    
    
    // Read the data from the files
    int N;
    int d;
    
    FILE *file1 = fopen("/Users/Maziar/Dropbox/GitHub/GeometricMedianADMM/MATLAB/Nd.txt", "r");
    if (file1 == NULL){
        perror("ERROR:");
    }
    
    int sizes[2];
    int readl;
    
    readl = fread_int(file1, sizes);
    if (readl != 2) {
        printf("I/O error in sizes.txt\n");
    }
    N = sizes[0];
    d = sizes[1];

    double* Z = (double*) malloc(sizeof(double)*N*d);
    double* w = (double*) malloc(sizeof(double)*N);
    
    
    file1 = fopen("/Users/Maziar/Dropbox/GitHub/GeometricMedianADMM/MATLAB/Z.txt", "r");
    if (file1 == NULL){
        perror("ERROR:");
    }
    readl = fread_float(file1, Z);
    if (readl != N*d)
        printf("I/O error in Z.txt\n");
    

    
    
    file1 = fopen("/Users/Maziar/Dropbox/GitHub/GeometricMedianADMM/MATLAB/w.txt", "r");
    if (file1 == NULL){
        perror("ERROR:");
    }
    readl = fread_float(file1, w);
    if (readl != N)
        printf("I/O error in w.txt\n");
    
    
    //show_row_rect_matrix("Z' is:", Z, N, d); // Z is saved column based
    
    //show_row_rect_matrix("w is:", w, N, 1);
    

    //time_t now;
    //time_t after;
    struct timespec start, finish;
    double elapsed;
    
    
    double* x_single = (double*) calloc(d, sizeof(double));
    algorithmStatistics_t singleT_stat[1];
    
    
    //now = time(0);
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    GeoMedianADMM_SingleThread(x_single, singleT_stat, Z, w, N, d);
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
    //after = time(0);
    
    //std::cout <<"\nElapsed time is: "<< after - now << " s"<< std::endl;
    //singleT_stat[0].time = after - now;
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    singleT_stat[0].time = elapsed;
    printStat("Stats for Single Thread", singleT_stat);
    
    //show_row_rect_matrix("x is:", x, d, 1);
    
    //std::cout << "Hello, World!\n";
    
    //-----------------------------------------------------------
    
    
#ifdef MultiThread
    double* x_multi = (double*) calloc(d, sizeof(double));
    algorithmStatistics_t multiT_stat[1];
    
    
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    //now = time(0);
    
    GeoMedianADMM_MultiThread(x_multi, multiT_stat, Z, w, N, d);
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
    //after = time(0);
    
    //std::cout <<"\nElapsed time is: "<< after - now << " s"<< std::endl;
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    multiT_stat[0].time = elapsed;
    printStat("Stats for Multi Thread", multiT_stat);
    
#endif// MultiThread
    
    
    
    
    return 0;
}
