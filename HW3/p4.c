#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
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
		{
			printf("%d ", M[i][j] );
		}
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
	//printf("Thread number %d working\n", p->line );
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
	//We will create n threads, one per line of the output
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

	int ** A = initialize_array(n, 1);
	int ** B = initialize_array(n, 1);

	int ** C_serial = initialize_array(n, 0);
	int ** C_pthreads = initialize_array(n, 0);
	int ** C_openmp = initialize_array(n, 0);

	struct timespec start, finish;
	double elapsed_serial;

	clock_gettime(CLOCK_MONOTONIC, &start);
	matrix_product_serial(n,C_serial,A,B);
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed_serial = (finish.tv_sec - start.tv_sec);
	elapsed_serial += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


	printf("Serial\t%f\n", elapsed_serial);
 
 	double elapsed_pthreads;
 	double elapsed_openmp;

 	printf("Speedup results\n");
	printf("nthreads\tpthreads\topenmp\n");
	for(int nthreads = 1; nthreads<=4; nthreads++)
	{
		clock_gettime(CLOCK_MONOTONIC, &start);
		matrix_product_pthreads(n,C_pthreads,A,B,nthreads);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		
		double tf, ts;
		ts = omp_get_wtime();
		matrix_product_openmp(n,C_openmp,A,B,nthreads);
		tf = omp_get_wtime();

		elapsed_pthreads = (finish.tv_sec - start.tv_sec);
		elapsed_pthreads += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
		
		elapsed_openmp = tf-ts;
	
		printf("%d\t\t%f\t%f\n", 	nthreads,
									elapsed_serial/elapsed_pthreads,
									elapsed_serial/elapsed_openmp);

		if(!(matrix_equal(n,C_serial,C_pthreads) && matrix_equal(n,C_serial,C_openmp)))
			printf("Test failed. Different result for serial and paralized multiplication methods\n");
	}
}




