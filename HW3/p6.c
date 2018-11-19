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


typedef struct params{
	int id;	

	int x_start;
	int x_finish;
	int y_start;
	int y_finish;

	int amount_work;

	int N;
	int ** pixels;
} Params;



void fill_pixels(void * args)
{

	Params *p = (Params *)args;

	double c_x = -0.7;
	double c_y = 0.26;
	double delta_x = 3.0/p->N;
	double delta_y = 2.0/p->N;

	double z_x;
	double z_y;

	int iteration;
	int MAX;


	int i = p->x_start;
	int j = p->y_start;

	for (int k = 0; k<p->amount_work; k++)
	{
		if(i == p->x_finish && j == p->y_finish)
			return;

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

		p->pixels[i][j] = iteration;


		if(j< p->N -1)
			j++;
		else
		{
			j=0;
			i++;
		}
	}
}


// Computes an N x N julia set, storing the results in the pixels matrix
// Using nthreads number of pthreads
// (pixel matrix allocated by caller, not within function)
void julia_set_pthreads( int N, int ** pixels, int nthreads )
{

	void *status;
	pthread_t t[nthreads];
	Params p[nthreads];
	
	for (int i=0;i<nthreads;i++)
	{
		p[i].id=i;
		p[i].N=N;
		p[i].pixels=pixels;

		//Assume nthreads << N, each thread will compute more than one line


		//Every thread starts when the previous thread stopped
		if(i==0)
		{
			p[i].x_start=0;
			p[i].y_start=0;	
		}
		else
		{
			p[i].x_start=p[i-1].x_finish;
			p[i].y_start=p[i-1].y_finish;
		}

		//Work of each thread = NxN/nthreads
		//First NxN%nthreads do +1 work

		if(i<(N*N)%nthreads)
			p[i].amount_work = N*N/nthreads +1;
		else
			p[i].amount_work = N*N/nthreads;

	
		p[i].x_finish = p[i].x_start + (p[i].y_start+p[i].amount_work)/N;
		p[i].y_finish = (p[i].y_start + p[i].amount_work)%N;


		pthread_create(&t[i], NULL, fill_pixels,(void*) &p[i]);
	}
	//Wait for all threads to finish
	for(int i=0;i<nthreads;i++) {
   		pthread_join(t[i], &status);
  	}

}


void main(int argc, char const *argv[])
{
	int n = 1000;

	int ** pixels_serial = initialize_array(n, 0);
	int ** pixels_pthreads = initialize_array(n, 0);

	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
	julia_set_serial(n,pixels_serial);
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Work time julia_set_serial:%f\n", elapsed);


	clock_gettime(CLOCK_MONOTONIC, &start);
	julia_set_pthreads(n,pixels_pthreads,2);
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Work time julia_set_pthreads:%f\n", elapsed);

	

	save_integer_matrix(n, pixels_pthreads, "p6_pixels.txt");


	if(matrix_equal(n,pixels_serial,pixels_pthreads))
		printf("Test passed. Same result for different methods.\n");
	else
		printf("Test failed. Different result for serial and paralized methods\n");

}
