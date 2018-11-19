#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>



int * initialize_array(int n, int fill)
{
	unsigned int mystate;
	mystate = time(NULL) + (unsigned int)clock();

	int * array = (int *) malloc(n*sizeof(int));
	if(fill)
	{
		for (int i=0; i<n;i++)
			array[i] = rand_r(&mystate)%10;
	}
	return array;
}

void print_array(int *M, int n)
{
	printf("[");
	for(int i =0; i<n; i++)
	{
		printf("%d", M[i]);
		if(i!=n-1)
			printf(", ");
	}
	printf("]\n");
}

int array_equal(int n, int * A, int *B)
{
	for (int i=0; i<n; i++)
	{
		if(A[i]!= B[i])
			return 0;
	}
	return 1;

}


void prefix_sum_serial(int * A, int * B, int n)
{
	B[0] = A[0];
	for(int i = 1; i < n; i++ )
		B[i] = B[i-1] + A[i];
}

typedef struct params{
	int id;
	int nthreads;
	int start;
	int finish;
	int N;
	int * A;
	int * B;
	struct params * allParams;
	int sum;

} Params;

int done_step_two = 0;
int done_step_four = 0;
int done_step_five = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_main = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_thread = PTHREAD_COND_INITIALIZER;


void thread_work_prefix_sum_parallel(void * args)
{
	Params *p = (Params *)args;
	Params *allParams = p->allParams;

	/*STEP 2: All threads independently execute the prefix algorithm on their subdomain of the problem, ignoring
	any numbers preceding their subdomain. They store the total sum for their subdomain in a location
	other threads can access.*/
	p->B[p->start] = p->A[p->start];

	for(int i = p->start+1; i < p->finish; i++ )
		p->B[i] = p->B[i-1] + p->A[i];

	
	//We increase the amoun of done, and wait for signal that everybody is ready
	pthread_mutex_lock( &mutex );
	done_step_two++;
	
	//Wake up the main thread (if it is sleeping) to test the value of done  
  	pthread_cond_signal( &cond_main );

  	//STEP 3: Wait for the signal that all threads are ready
	pthread_cond_wait( & cond_thread, & mutex ); 
	pthread_mutex_unlock( & mutex );
	


	/*STEP 4: All threads compute the sum of all subdomains preceding them by way of adding together the total
	subdomain sums computed in step */
	int sum=0;
	for(int i=0; i<p->id;i++)
	{
		//printf("p[%d] working: %d\n", p->id, p->B[allParams[i].finish - 1]);
		sum += p->B[allParams[i].finish - 1];
	}
	p->sum=sum;

	//We increase the amoun of done, and wait for signal that everybody is ready
	pthread_mutex_lock( &mutex );
	done_step_four++;
	
	//wake up the main thread (if it is sleeping) to test the value of done  
  	pthread_cond_signal( &cond_main );

  	//Wait for the signal that all threads are ready
	pthread_cond_wait( & cond_thread, & mutex ); 
	pthread_mutex_unlock( & mutex );
	


	//STEP 5. All threads add the sum computed in step (4) to all elements
	//in their subdomain, and store their results in the output vector.
	for(int i=p->start; i<p->finish; i++)
	{
		p->B[i]+=p->sum;
	}

	//We increase the amoun of done, and wait for signal that everybody is ready
	pthread_mutex_lock( &mutex );
	done_step_five++;
	
	//wake up the main thread (if it is sleeping) to test the value of done  
  	pthread_cond_signal( &cond_main );

  	//Wait for the signal that all threads are ready
	pthread_cond_wait( & cond_thread, & mutex ); 
	pthread_mutex_unlock( & mutex );
	
}




void prefix_sum_parallel(int * A, int * B, int N, int nthreads)
{
	done_step_two = done_step_four = done_step_five = 0;

	void *status;
	pthread_t t[nthreads];
	Params p[nthreads];
	
	//STEP 1: Decompose problem in subdomains
	for (int i=0;i<nthreads;i++)
	{
		p[i].id=i;
		p[i].nthreads=nthreads;
		p[i].N=N;
		p[i].A=A;
		p[i].B=B;

		p[i].allParams = &p;

		//Lets assume nthreads << number of elements in array
		//Every thread starts when the previous thread stopped
		if(i==0)
			p[i].start=0;	
		else
			p[i].start=p[i-1].finish;

		//Every thread works on a subdomain of N/nthreads elements
		//except the N%nthreads first threads that work N/nthreads+1 elements.
		if(i<N%nthreads)
			p[i].finish=p[i].start+N/nthreads+1;
		else
			p[i].finish=p[i].start+N/nthreads;

		//Threads execute step 2
		pthread_create(&t[i], NULL, thread_work_prefix_sum_parallel,(void*) &p[i]);
	}


	//Checking step 2 is done for all threads...
	//Working on done, need to mutex
	pthread_mutex_lock( &mutex );
	//Are threads finished in the first sept?
	while(done_step_two < nthreads)
	{
	  /* block this thread until another thread signals cond. While
		blocked, the mutex is released, then re-aquired before this
		thread is woken up and the call returns. */ 
	  pthread_cond_wait( & cond_main, & mutex ); 
	  
	  /* we go around the loop with the lock held */
	}


	//Lets tell everyone to go back to life
	pthread_cond_broadcast( &cond_thread );
	pthread_mutex_unlock( & mutex );


	//Checking step 4 is done for all threads...
	pthread_mutex_lock( &mutex );
	while(done_step_four < nthreads)
	{	  
	  pthread_cond_wait( & cond_main, & mutex ); 
 	  /* we go around the loop with the lock held */
	}

	pthread_cond_broadcast( &cond_thread );
	pthread_mutex_unlock( & mutex );
	
	//Checking step 5 is done for all threads...
	pthread_mutex_lock( &mutex );
	while(done_step_five < nthreads)
	{	  
	  pthread_cond_wait( & cond_main, & mutex ); 
	  /* we go around the loop with the lock held */
	}

	pthread_cond_broadcast( &cond_thread );
	pthread_mutex_unlock( & mutex );
	  
	//We are finished, join the threads
	for(int i=0;i<nthreads;i++)
   		pthread_join(t[i], &status);
}






int main ()
{
	int n=100000000;

	int* A = initialize_array(n,1);
	
	int* B_serial = initialize_array(n,0);
	int* B_parallel = initialize_array(n,0);

	struct timespec start, finish;
	double elapsed_serial;
	double elapsed_parallel;

	clock_gettime(CLOCK_MONOTONIC, &start);	
	prefix_sum_serial(A,B_serial,n);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	
	elapsed_serial = (finish.tv_sec - start.tv_sec);
	elapsed_serial += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	

	printf("Work time prefix_sum_serial %f\n", elapsed_serial);

	printf("nthreads\twork time(sec)\tspeedup\n");
	for(int nthreads = 1; nthreads<=4; nthreads++)
	{

		clock_gettime(CLOCK_MONOTONIC, &start);	
		prefix_sum_parallel(A,B_parallel,n, nthreads);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		
		elapsed_parallel = (finish.tv_sec - start.tv_sec);
		elapsed_parallel += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
		
		printf("%d\t\t%f\t%f\n", nthreads, elapsed_parallel, elapsed_serial/elapsed_parallel);
	
		if(!array_equal(n,B_serial,B_parallel))
			printf("Test failed. Different result for serial and paralized methods\n");
	}


}