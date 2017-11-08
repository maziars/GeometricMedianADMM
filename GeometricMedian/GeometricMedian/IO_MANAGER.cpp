//
//  IO_MANAGER.cpp
//  PCG
//
//  Created by Maziar Sanjabi on 12/25/15.
//  Copyright © 2015 Maziar Sanjabi. All rights reserved.
//

#include "IO_MANAGER.hpp"
//FILE *file = fopen("Integers.txt", "r");
//int integers[100];



int fread_int(FILE *file, int *integers){
    
    int i=0;
    int num;
    while(fscanf(file, "%d", &num) > 0) {
        integers[i] = num;
        i++;
    }
    fclose(file);
    return i;
    
}


int fread_float(FILE *file, double *floats){
    
    int i=0;
    double num;
    while(fscanf(file, "%lf", &num) > 0) {
        floats[i] = num;
        i++;
    }
    fclose(file);
    return i;
}



void show_row_rect_matrix(char* desc, double* A, int nRows, int nCols)
{
    printf("\n%s\n", desc);
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++)
            printf("%2.15f ", A[i * nCols + j]);
        printf("\n");
    }
    printf("\n---------\n");
}
