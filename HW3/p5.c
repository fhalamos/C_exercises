#include<stdio.h>
#include<stdlib.h>
#include<time.h>

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
			printf("%d\t", M[i][j] );
		printf("\n");
	}
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

void main(int argc, char const *argv[])
{
	int n = 1000;

	int ** pixels = initialize_array(n, 0);

	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
	julia_set_serial(n,pixels);
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Work time julia_set_serial:%f\n", elapsed);

	save_integer_matrix(n, pixels, "p5_pixels.txt");
}
