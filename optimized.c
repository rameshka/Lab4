#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"
#include  <omp.h>

int Rand_Max = 2500;
void populate_row_major(double* matrix, int size);
void populate_column_major(double* matrix, int size);
void metrixMultiplication(double* matrx_1,double* matrix_2, double* matrix_3, int size);
void printMatrix(double* matrix,int size);

int main(int argc, char **argv) {
  /* code */
  int  size = (int)strtol(argv[1], NULL, 10);
  double start, finish;
  double *matrix_A;
  double *matrix_B;
  double *matrix_C;
  srand(time(NULL)); //seeding rand function

  matrix_A = (double *)malloc(size*size * sizeof(double));//allocating memory
  matrix_B = (double *)malloc(size*size * sizeof(double));
  matrix_C = (double *)malloc(size*size * sizeof(double));

  populate_row_major(matrix_A, size);
  populate_column_major(matrix_B, size);

  GET_TIME(start);

  metrixMultiplication(matrix_A,matrix_B,matrix_C, size);

  GET_TIME(finish);
      //Calculate and Display the execution time
  printf("Time spent for parallel optimized : %lf seconds\n", (finish - start) );

  free(matrix_A);
  free(matrix_B);
  free(matrix_C);

  return 0;
}

//as we are assiging values to the matrix populating in column major and row major are identical

void populate_row_major(double* matrix, int size){
  #pragma omp parallel for
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      double temp =(double)(rand()%Rand_Max);
      matrix[size*i+j] = (double)(rand()%Rand_Max);
    //  printf("from thread %d\n",omp_get_thread_num() );
    }
  }
}

void populate_column_major(double* matrix, int size){
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

  double temp;
  int a;
  int b;
  #pragma omp parallel for private(temp,a,b)
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      temp = 0.0;
      for(int k = 0; k < size; k+=5){
        a = i*size + k;
        b = j*size + k;
      temp = temp   + matrx_1[a] * matrix_2[b]
                    + matrx_1[a+1] * matrix_2[b+1]
                    + matrx_1[a+2] * matrix_2[b+2]
                    + matrx_1[a+3] * matrix_2[b+3]
                    + matrx_1[a+4] * matrix_2[b+4];
      }
        matrix_3[i*size + j] = temp;
    }

  }
}
