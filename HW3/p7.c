#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include <limits.h>

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
			printf("%d\t", M[i][j] );
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


// Saves an integer matrix of size N x N to file
void save_integer_matrix( int N, int ** pixels, char * fname )
{

   	FILE *fp;
   	fp = fopen(fname, "w+");
   
	for(int i =0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{

	   		char str_i[12];
			sprintf(str_i, "%d ", i);		
			fputs(str_i, fp);


			char str_j[12];
			sprintf(str_j, "%d ", j);		
			fputs(str_j, fp);

			char str_pixel[12];
			sprintf(str_pixel, "%d", pixels[i][j]);		
			fputs(str_pixel, fp);

			fputs("\n", fp);			
		}

	}
	fclose(fp);
}


// Computes an N x N julia set, storing the results in the 2D pixels matrix
// (pixel matrix allocated by caller, not within function)
void julia_set_serial( int N, int ** pixels )
{
	double c_x = -0.7;
	double c_y = 0.26;
	double delta_x = 3.0/N;
	double delta_y = 2.0/N;

	double z_x;
	double z_y;

	int iteration;
	int MAX;

	for (int i =0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			z_x = -1.5 + delta_x*i;
			z_y = -1.0 + delta_y*j;

			iteration = 0;
			MAX = 1000;

			while(z_x*z_x + z_y*z_y < 4.0 && iteration<= MAX)
			{
				double tmp = z_x*z_x - z_y*z_y;
				z_y = 2*z_x*z_y + c_y;
				z_x = tmp + c_x;
				iteration+=1;
			}

			pixels[i][j] = iteration;
		}
	}
}




// Computes an N x N julia set, storing the results in the pixels matrix
// Using nthreads number of OpenMP threads
// (pixel matrix allocated by caller, not within function)

// Use OpenMP dynamic load balancer.
// Experiment with work chunk sizes to determine what is approximately optimal on
// your system when using all available threads.
void julia_set_openmp( int N, int ** pixels, int nthreads, int chuncksize )
{

	#pragma omp parallel num_threads(nthreads)
	{
		
		int my_id; /* variables declared in a parallel block are private
			  to each thread -- ie one copy per thread */
	    my_id = omp_get_thread_num();

		double c_x = -0.7;
		double c_y = 0.26;
		double delta_x = 3.0/N;
		double delta_y = 2.0/N;

		double z_x;
		double z_y;

		int iteration;
		int MAX;

		#pragma omp for schedule(dynamic,chuncksize)
			for (int i =0; i<N; i++)
			{
				for(int j=0; j<N; j++)
				{

					//printf("Thread %d working on [%d,%d]\n", my_id, i, j);					
					z_x = -1.5 + delta_x*i;
					z_y = -1.0 + delta_y*j;

					iteration = 0;
					MAX = 1000;

					while(z_x*z_x + z_y*z_y < 4.0 && iteration<= MAX)
					{
						double tmp = z_x*z_x - z_y*z_y;
						z_y = 2*z_x*z_y + c_y;
						z_x = tmp + c_x;
						iteration+=1;
					}

					pixels[i][j] = iteration;
				}
			}
	}
}

void main(int argc, char const *argv[])
{
	int n = 1000;

	int ** pixels_serial = initialize_array(n, 0);
	int ** pixels_openmp = initialize_array(n, 0);

	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
	julia_set_serial(n,pixels_serial);
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Work time julia_set_serial:%f\n", elapsed);

	printf("Work time julia_set_openmp\n");
	printf("Chuncksize\tWork time\n");

	double min_time = INT_MAX;

	double tf, ts;

	for(int chuncksize=1; chuncksize< 100; chuncksize++)
	{
		ts = omp_get_wtime();
		julia_set_openmp(n,pixels_openmp,4, chuncksize);
		tf = omp_get_wtime();
		elapsed=tf-ts;

		printf("%d\t%f\n", chuncksize, elapsed);
		
		if(elapsed<min_time)
		{
			printf("Found new best chuncksize = %d, work time= %f\n", chuncksize, elapsed);
			min_time = elapsed;
			save_integer_matrix(n, pixels_openmp, "p7_pixels.txt");			
		}

	}
	



	if(matrix_equal(n,pixels_serial,pixels_openmp))
		printf("Test passed. Same result for different methods.\n");
	else
		printf("Test failed. Different result for serial and paralized methods\n");

}
