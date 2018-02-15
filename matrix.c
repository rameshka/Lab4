#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"
#include  <omp.h>

int Rand_Max = 2500;
void populate(double* matrix, int size);
void metrixMultiplication(double* matrx_1,double* matrix_2, double* matrix_3, int size);
void printMatrix(double* matrix,int size);

int main() {
  /* code */
  int  size = 2000;
  double start, finish;
  double *matrix_A;
  double *matrix_B;
  double *matrix_C;
  srand(time(NULL)); //seeding rand function

  matrix_A = (double *)malloc(size*size * sizeof(double));
  matrix_B = (double *)malloc(size*size * sizeof(double));
  matrix_C = (double *)malloc(size*size * sizeof(double));

  populate(matrix_A, size);
  populate(matrix_B, size);

  GET_TIME(start);

  metrixMultiplication(matrix_A,matrix_B,matrix_C, size);

  GET_TIME(finish);
      //Calculate and Display the execution time
  printf("Time spent for parallel  : %lf seconds\n", (finish - start) );

  free(matrix_A);
  free(matrix_B);
  free(matrix_C);

  return 0;
}

void populate(double* matrix, int size){

  #pragma omp parallel for
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      double temp =(double)(rand()%Rand_Max);
      matrix[size*i+j] = (double)(rand()%Rand_Max);
    //  printf("from thread %d\n",omp_get_thread_num() );
    }
  }
}

void printMatrix(double* matrix,int size){
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf("%lf ",matrix[size*i+j]);
    }
    printf("\n");
  }
}

void metrixMultiplication(double* matrx_1,double* matrix_2,double* matrix_3, int size){
  #pragma omp parallel for 
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      matrix_3[i*size + j] = 0.0;
      for(int k = 0; k < size; k++){
      matrix_3[i*size + j] = matrix_3[i*size + j] + matrx_1[i*size + k] * matrix_2[k*size+j];
      }
    }

  }
}
