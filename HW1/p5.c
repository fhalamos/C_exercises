#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <time.h>
#define M 1000
#define N 1000



double **dmatrix_non_contiguous(size_t m, size_t n)
{
	/*ptList points to the beggining of a list of m elements,
	Each of those elements has to be big enough to save
	a pointer to a list of doubles*/
	double ** ptList;
	ptList = (double **) malloc(m*sizeof(double*));
	int i;
	/*For each of the m pointers*/
	for (i=0;i<m;i++)
		ptList[i] = (double*) malloc(n*sizeof(double));

	return ptList;
}

double **dmatrix_contiguous(size_t m, size_t n)
{
	/*Pointer to the matrix. It has m elements, each
	of them is a pointer to a list of doubles*/
	double ** ptList;
	ptList = (double **) malloc(m*sizeof(double *));

	/*We reserve the necessary space considering all m lists one
	after the other
	Each of the m lists saves n elements*/
	ptList[0] = (double *) malloc (m*n*sizeof(double));


	/*
    *ptList = returns value to which ptList is pointing.
    		= value of the address where the first pointer of double* is.
    
	If we wont data to be contiguous, the value of the next pointer
	must be n values bigger than *ptList... and keep going with the next ones
    */	
	int i;
    for(i = 0; i < m; i++) 
        ptList[i] = (*ptList + n * i);

	return ptList;
}

void work_kernel_dynamic(double **U, int m, int n)
{
	int i,j;
	double a = 1.0, b = 0.5, c = 0.0;

	for( i=1; i < (m-1); i++)
		for( j=1; j < (n-1); j++)
		{
			c += ( a * (U[i+1][j] + U[i-1][j]) ) +
				( b * (U[i][j+1] + U[i][j-1]) ) +
				( (1 - (2*a) - (2*b)) * U[i][j] );
		}
	printf("c: %.2f\n", c);
	return;
}

void work_kernel_static(double U[M][N], int m, int n)
{
	int i,j;
	double a = 1.0, b = 0.5, c = 0.0;

	for( i=1; i < (m-1); i++)
		for( j=1; j < (n-1); j++)
		{
			c += ( a * (U[i+1][j] + U[i-1][j]) ) +
				( b * (U[i][j+1] + U[i][j-1]) ) +
				( (1 - (2*a) - (2*b)) * U[i][j] );
		}
	printf("c: %.2f\n", c);
	return;
}

int main(int argc, char * argv[])
{
	int m = 1000;
	int n = 1000;

	/* Declare and allocate your arrays A,B,C */
	double A[m][n];
	double **B = dmatrix_non_contiguous(m,n);
	double **C = dmatrix_contiguous(m,n);

	/* Initialize arrays */
	int i;
	for(i=0;i<m;i++){
		int j;
		for(j=0;j<n;j++){
			double value = rand()/(double)RAND_MAX;
			A[i][j] = value;
			B[i][j] = value;
			C[i][j] = value;

		}
	}


	clock_t start_t, stop_t;
	double A_work_time, B_work_time, C_work_time;

	/* time A */
	start_t = clock();    
	work_kernel_static(A, m, n);
	stop_t = clock();
 	A_work_time = (double)(stop_t - start_t) / CLOCKS_PER_SEC;
 
	/* time B */
	start_t = clock();
	work_kernel_dynamic( B, m, n);
	stop_t = clock();
 	B_work_time = (double)(stop_t - start_t) / CLOCKS_PER_SEC;

	/* time C */
	start_t = clock();
	work_kernel_dynamic( C, m, n);
	stop_t = clock();
 	C_work_time = (double)(stop_t - start_t) / CLOCKS_PER_SEC;	


	/* Print out timing to stdout in seconds for each array */
 	printf("A took: %f seconds\n", A_work_time);
 	printf("B took: %f seconds\n", B_work_time);
 	printf("C took: %f seconds\n", C_work_time);
	
	/* Free memory*/
	free(B);
	free(C);

	return 0;

}
