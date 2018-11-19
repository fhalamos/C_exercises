#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>


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
			printf("%d ", M[i][j] );
		printf("\n");
	}
}

int matrix_equal(int n, int ** C, int **D)
{
	for (int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			if(C[i][j] != D[i][j])
				return 0;
		}
	}
	return 1;
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
// Computes the 2D matrix product C = AB, with matrix dimensions N x N
// using nthreads number of OpenMP threads
void matrix_product_openmp( int N, int ** C, int ** A, int ** B, int nthreads )
{
	#pragma omp parallel num_threads(nthreads)
	{		
		int sum =0;
		#pragma omp for 
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
}





void main(int argc, char const *argv[])
{
	/* Intializes random number generator */
 	time_t t;
  	srand((unsigned) time(&t));

	int n = atoi(argv[1]);
	int nthreads = atoi(argv[2]);

	int ** A = initialize_array(n, 1);
	int ** B = initialize_array(n, 1);

	int ** C_serial = initialize_array(n, 0);
	int ** C_openmp = initialize_array(n, 0);


	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
	matrix_product_serial(n,C_serial,A,B);
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Work time matrix_product_serial:%f\n", elapsed);
 

	double tf, ts;

	ts = omp_get_wtime();

	matrix_product_openmp(n,C_openmp,A,B,nthreads);

	tf = omp_get_wtime();

	printf("Work time matrix_product_openmp:%f\n", tf-ts);


	if(matrix_equal(n,C_serial,C_openmp))
		printf("Test passed. Same result for different multiplication methods.\n");
	else
		printf("Test failed. Different result for serial and paralized multiplication methods\n");
}




