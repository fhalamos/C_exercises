#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

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

typedef struct params{
	int start_line;
	int finish_line;
	int N;
	int ** A;
	int ** B;
	int ** C;
} Params;

void multiply_line_and_column(void * args)
{
	Params *p = (Params *)args;
	int sum=0;	
	for (int i=p->start_line; i<p->finish_line;i++)
	{
		for(int j=0; j<p->N; j++)
		{
			for(int k=0; k<p->N; k++)	
				sum += p->A[i][k]*p->B[k][j];
			p->C[i][j]=sum;
			sum=0;
		}
	}
}

// Computes the 2D matrix product C = AB, with matrix dimensions N x N
// using nthreads number of pthreads
void matrix_product_pthreads( int N, int ** C, int ** A, int ** B, int nthreads )
{
	void *status;
	pthread_t t[nthreads];
	Params p[nthreads];
	
	for (int i=0;i<nthreads;i++)
	{
		p[i].N=N;
		p[i].A=A;
		p[i].B=B;
		p[i].C=C;

		//If nthreads = N, each thread computes one line
		if(nthreads==N)
		{
			p[i].start_line=i;
			p[i].finish_line=i+1;
		}
		//If nthreads > N, the nthreads-N last threads will have no work.
		else if(nthreads>N)
		{
			p[i].start_line=i;
			if(i<N)
				p[i].finish_line=i+1;
			else
				p[i].finish_line=i;	
		}	
		//If nthreads < N, each thread will compute more than one line
		else
		{
			//Every thread starts when the previous thread stopped
			if(i==0)
				p[i].start_line=0;	
			else
				p[i].start_line=p[i-1].finish_line;

			//Every thread works on N/nthreads
			//except the N%nthreads first threads that work N/nthreads+1 lines.
			if(i<N%nthreads)
				p[i].finish_line=p[i].start_line+N/nthreads+1;
			else
				p[i].finish_line=p[i].start_line+N/nthreads;

		}

		pthread_create(&t[i], NULL, multiply_line_and_column,(void*) &p[i]);
	}
	//Wait for all threads to finish
	for(int i=0;i<nthreads;i++) {
   		pthread_join(t[i], &status);
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
	int ** C_pthreads = initialize_array(n, 0);

	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
	matrix_product_serial(n,C_serial,A,B);
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Work time matrix_product_serial:%f\n", elapsed);

	clock_gettime(CLOCK_MONOTONIC, &start);
	matrix_product_pthreads(n,C_pthreads,A,B,nthreads);
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Work time matrix_product_pthreads:%f\n", elapsed);

	if(matrix_equal(n,C_serial,C_pthreads))
		printf("Test passed. Same result for different multiplication methods.\n");
	else
		printf("Test failed. Different result for serial and paralized multiplication methods\n");
}
