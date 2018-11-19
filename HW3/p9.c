#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define N 1000000


double pi_serial()
{
	double num_hits =0;

	unsigned int mystate;
	mystate = time(NULL) + (unsigned int)clock();
		

	//Generate 1,000,000 samples
	for(int i=0; i<N; i++)
	{
		//We will work in a 1x1 square
		//Generate sample point
		double x = (double)rand_r(&mystate)/RAND_MAX;
		double y = (double)rand_r(&mystate)/RAND_MAX;

		//If the distance from the random point to center of circle
		//is smaller than circle radio, increase counter hits
		if(x*x + y*y<=1)
			num_hits++;
	}
	return 4*num_hits/N;
}


double pi_parallel_automatic_reduction()
{
	double num_hits =0;

	#pragma omp parallel reduction(+:num_hits)
	{
		unsigned int mystate;
	  	mystate = time(NULL) + omp_get_thread_num() + (unsigned int)clock();

		//int my_id = omp_get_thread_num();
		//printf("hola soy thread %d\n", my_id );
		
		//Generate 1,000,000 samples
		#pragma omp for
		for(int i=0; i<N; i++)
		{

			double x = (double)rand_r(&mystate)/RAND_MAX;
			double y = (double)rand_r(&mystate)/RAND_MAX;

			if(x*x + y*y<=1)
			{
				num_hits++;
			}
		}
	}
	return 4*num_hits/N;

}

/*
Manual Reduction B – Inside of the parallel region, but outside of the OpenMP for loop, declare a
new local num hits local state variable that is private. Inside the OpenMP for loop, have threads
increment their local state num hits local variables instead of the global one. Finally, once the
OpenMP for loop has completed, but you are still inside the parallel region, use an OpenMP atomic
to protect an add of the local state variable(num hits local) to the global state variable (num hits)
for all threads
*/

double pi_parallel_manual_reduction_b()
{
	double num_hits =0;


	#pragma omp parallel shared(num_hits)
	{
		double num_hits_local=0;

		// XOR multiple values together to get a semi-unique seed
		unsigned int mystate;
	  	mystate = time(NULL) + omp_get_thread_num() + (unsigned int)clock();
		
		//int my_id = omp_get_thread_num();
		//printf("hola soy thread %d\n", my_id );
		
		//Generate 1,000,000 samples
		#pragma omp for 
		for(int i=0; i<N; i++)
		{

			double x = (double)rand_r(&mystate)/RAND_MAX;
			double y = (double)rand_r(&mystate)/RAND_MAX;

			if(x*x + y*y<=1)
			{
				num_hits_local++;
			}
		}

		#pragma omp atomic
			num_hits += num_hits_local;

	}
	return 4*num_hits/N;
}

/*
Manual Reduction A – Instead of having a single num hits state variable, allocate an array of state
variables (of length nthreads) outside the parallel region. Then, have each OpenMP thread increment
the state variable that corresponds to its thread id (as queried by omp get thread num()). Finally,
once you have exited the parallel region of the code, sum the state variables together to compute your
final answer for num hits.
*/

double pi_parallel_manual_reduction_a(int nthreads)
{
	int *num_hits_array = (int *) malloc(nthreads* sizeof(int)); 
	for(int i=0; i<nthreads;i++)
		num_hits_array[i]=0;
	

	#pragma omp parallel shared(num_hits_array)
	{
		unsigned int mystate;
	  	mystate = time(NULL) + omp_get_thread_num() + (unsigned int)clock();

		//int my_id = omp_get_thread_num();
		//printf("hola soy thread %d\n", my_id );
		
		//Generate 1,000,000 samples
		#pragma omp for 
		for(int i=0; i<N; i++)
		{

			double x = (double)rand_r(&mystate)/RAND_MAX;
			double y = (double)rand_r(&mystate)/RAND_MAX;

			if(x*x + y*y<=1)
			{
				num_hits_array[omp_get_thread_num()]++;
			}
		}
	}

	double num_hits=0;

	for(int i=0; i<nthreads; i++)
		num_hits+=num_hits_array[i];
	
	return 4*num_hits/N;


}

double pi_parallel_with_atomic()
{
	double num_hits =0;


	#pragma omp parallel shared(num_hits)
	{
		// XOR multiple values together to get a semi-unique seed
		unsigned int mystate;
	  	mystate = time(NULL) + omp_get_thread_num() + (unsigned int)clock();
		
		//int my_id = omp_get_thread_num();
		//printf("hola soy thread %d\n", my_id );
		
		//Generate 1,000,000 samples
		#pragma omp for 
		for(int i=0; i<N; i++)
		{

			double x = (double)rand_r(&mystate)/RAND_MAX;
			double y = (double)rand_r(&mystate)/RAND_MAX;

			if(x*x + y*y<=1)
			{
			#pragma omp atomic
				num_hits++;
			}
		}
	}
	return 4*num_hits/N;
}

double pi_parallel_with_critical()
{
	double num_hits =0;

	#pragma omp parallel shared(num_hits)
	{
		unsigned int mystate;
	  	mystate = time(NULL) + omp_get_thread_num() + (unsigned int)clock();

		//int my_id = omp_get_thread_num();
		//printf("hola soy thread %d\n", my_id );
		
		//Generate 1,000,000 samples
		#pragma omp for 
		for(int i=0; i<N; i++)
		{

			double x = (double)rand_r(&mystate)/RAND_MAX;
			double y = (double)rand_r(&mystate)/RAND_MAX;

			if(x*x + y*y<=1)
			{
			#pragma omp critical
				num_hits++;
			}
		}
	}
	return 4*num_hits/N;
}

void print_results(double pi_result, double finish, double start)
{
	printf("Pi obtained: \t\t%f\n", pi_result);
	double error = fabs(pi_result - M_PI);
	printf("Error: \t\t\t%f, %.4f%%\n", error, error/M_PI*100);
	printf("Time to compute: \t%f sec\n\n", finish-start );
}

int main ()
{
	double start, finish;
	double pi_result;

	printf("->Serial method\n");
	start = omp_get_wtime();
	pi_result = pi_serial();
	finish = omp_get_wtime();

	print_results(pi_result, finish, start);
	
	printf("->With atomic\n");
	start = omp_get_wtime();
	pi_result = pi_parallel_with_atomic();
	finish = omp_get_wtime();
	print_results(pi_result, finish, start);

	printf("->With critical\n");
	start = omp_get_wtime();
	pi_result = pi_parallel_with_critical();
	finish = omp_get_wtime();
	print_results(pi_result, finish, start);

	printf("->Manual reduction a\n");
	start = omp_get_wtime();
	pi_result = pi_parallel_manual_reduction_a(4);
	finish = omp_get_wtime();
	print_results(pi_result, finish, start);

	printf("->Manual reduction b\n");
	start = omp_get_wtime();
	pi_result = pi_parallel_manual_reduction_b();
	finish = omp_get_wtime();
	print_results(pi_result, finish, start);


	printf("->Automatic reduction\n");
	start = omp_get_wtime();
	pi_result = pi_parallel_automatic_reduction();
	finish = omp_get_wtime();
	print_results(pi_result, finish, start);



}