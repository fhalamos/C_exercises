#include<stdio.h>
#include<stdlib.h>

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

int main(int argc, char **argv){

	size_t m=0,n=0,i,j;
	double **matrix;

	n = atoi(argv[1]);
	m = n;
	matrix = dmatrix_contiguous(m,n);

	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			matrix[i][j] = i*n+j;
		}
	}

	printf("Matrix with 1D indexing:\n");
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			printf("%.1f ", matrix[0][i*n + j]);
		}
		putchar('\n');
	}
	putchar('\n');

	printf("Matrix with 2D indexing:\n");
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			printf("%.1f ", matrix[i][j]);
		}
		putchar('\n');
	}

	return 0;
	
}

