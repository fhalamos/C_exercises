#include<stdio.h>
#include<stdlib.h>
#include <time.h>

//fill==1 if we want to return the matrix filled with random numbers from 0 to 9
int ** initialize_array(int n, int fill )
{
	int ** array = (int **) malloc(n*sizeof(int*));
	for (int i=0; i<n;i++)
	{
		array[i] = (int *) malloc(n*sizeof(int));
		if(fill)
		{
			for(int j=0; j<n; j++)
				array[i][j] = rand()%10;
		}
   
	}
	return array;
}

void print_matrix(int **M, int n)
{
	for(int i =0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			printf("%d ", M[i][j] );
		}
		printf("\n");
	}
}

// Computes the 2D matrix product C = AB, with matrix dimensions N x N
void matrix_product_serial( int N, int ** C, int ** A, int ** B )
{
	int sum=0;
	for(int i =0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			for(int k=0; k<N; k++)	
				sum += A[i][k]*B[k][j]; //multiply whole row i of A with column j of B 
			C[i][j]=sum;
			sum=0;
		}
	}
}

void main(int argc, char const *argv[])
{
	/* Intializes random number generator */
   	time_t t;
   	srand((unsigned) time(&t));

	int n = atoi(argv[1]);

	int ** A = initialize_array(n, 1);
	int ** B = initialize_array(n, 1);
	int ** C = initialize_array(n, 0);

	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
	matrix_product_serial(n,C,A,B);
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Work time matrix_product_serial:%f\n", elapsed);
}