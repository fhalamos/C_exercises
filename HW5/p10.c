#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include<time.h>



//Return nxn matrix
double ** generate_matrix(int n)
{
	double ** matrix = (double**) malloc(n*sizeof(double*));
	for (int i = 0; i < n; ++i)
		matrix[i]= (double*)malloc(n*sizeof(double));

	double a=rand()%10+1;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			matrix[i][j]=a;
			a=rand()%10+1;
		}
	}
	return matrix;
}

double* generate_vector(int n)
{
	double* b = (double*) malloc(n*sizeof(double));
	for (int i = 0; i < n; ++i)
		b[i]=rand()%10+1;
	return b;
}

void print_augmented_matrix(double ** m, int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j <= n; ++j)
		{
			printf("%.2f\t", m[i][j]);
		}
		printf("\n");
	}
}

double **generate_augmented_matrix(double ** m, double* b, int n )
{
	double ** augmented = (double**) malloc(n*sizeof(double*));
	for (int i = 0; i < n; ++i)
		augmented[i] = (double*) malloc((n+1)*sizeof(double));

	//Copy elements of m
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			augmented[i][j] = m[i][j];
		}
	}
	//Copy elements of b
	for (int i = 0; i < n; ++i)
	{
		augmented[i][n] = b[i];
	}
	return augmented;
}

void free_matrix(double ** m, int rows)
{
	for (int i = 0; i < rows; ++i)
		free(m[i]);
	free(m);
}

 int check_solution(double ** m, double* b, double* x, int n)
 {
 	for (int i = 0; i < n; ++i)
 	{
 		double sum=0;
 		for (int j = 0; j < n; ++j)
 		{
 			sum+=m[i][j]*x[j];
 		}

 		if((int)(sum-b[i])!=0)
 		{
 			//printf("Wrong solution. sum = %f, b[%d] = %f \n", sum, i ,b[i]);
 			return 0;
 		}
 	}
 	
 	return 1;
 	//printf("Solution checked and correct.\n");
 }

//Solve the system using Gaussian elimination with back-substitution
double GE_solve(double ** m, double* b, int n)
{
	double solution[n];

	//Step 0: Generate the augmented matrix
	double ** matrix = generate_augmented_matrix(m,b,n);
	//printf("Original augmented matrix:\n");
    //print_augmented_matrix(matrix,n);
    //printf("\n");

	//Gaussian eliminarion
	//Traverse the matrix through columns.
	//In each iteration we will leave only zeros bellow pivot in column j
	//After finishing loop we will have an upper triangular matrix

	struct timespec start, finish;
	double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);

    double c;
    for(int j=0; j<n; j++) 
    {
        for(int i=j+1; i<n; i++)
        {
            //c is the amount of times we need to substract row j to tow i
            c=matrix[i][j]/matrix[j][j];
            for(int k=0; k<=n; k++)
            {
                matrix[i][k]=matrix[i][k]-c*matrix[j][k];
            }
        }
    }
	//printf("Upper triangular augmented matrix:\n");
	//print_augmented_matrix(matrix,n);
	//printf("\n");

	//Back sustitution
	solution[n-1]=matrix[n-1][n]/matrix[n-1][n-1];
    for(int i=n-2; i>=0; i--)
    {
        double sum=0;
        for(int j=i+1; j<n; j++)
        {
            sum=sum+matrix[i][j]*solution[j];
        }
        solution[i]=(matrix[i][n]-sum)/matrix[i][i];
    }
    //for (int i = 0; i < n; ++i)
	//    printf("x[%d] = %f\n", i, solution[i]);

	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	
    free_matrix(matrix,n);

	//If there is no solution, generate new matrix A and vector b
	if(!check_solution(m,b,solution,n))
		return -1;

    return elapsed;
}

double GE_solve_parallel(double ** m, double* b, int n, int nthreads)
{
	double solution[n];
	//Step 0: Generate the augmented matrix
	double ** matrix = generate_augmented_matrix(m,b,n);
	//printf("Original augmented matrix:\n");
    //print_augmented_matrix(matrix,n);
    //printf("\n");

	//Gaussian eliminarion
	//Traverse the matrix through columns.
	//In each iteration we will leave only zeros bellow pivot in column j
	//After finishing loop we will have an upper triangular matrix
	double tf, ts;
	ts = omp_get_wtime();

	  
    //We cannot parallelize the columns-word because each process works on data build by the previous one
    for(int j=0; j<n; j++) 
    {
    #pragma omp parallel for
        for(int i=j+1; i<n; i++)
        {
            //c is the amount of times we need to substract row j to tow i
            double c=matrix[i][j]/matrix[j][j];
            for(int k=0; k<=n; k++)
            {
                matrix[i][k]=matrix[i][k]-c*matrix[j][k];
            }
        }
    }

	//printf("Upper triangular augmented matrix:\n");
	//print_augmented_matrix(matrix,n);
	//printf("\n");

	//Back sustitution
	solution[n-1]=matrix[n-1][n]/matrix[n-1][n-1];
    for(int i=n-2; i>=0; i--)
    {
        double sum=0;
        for(int j=i+1; j<n; j++)
        {
            sum=sum+matrix[i][j]*solution[j];
        }
        solution[i]=(matrix[i][n]-sum)/matrix[i][i];
    }
    
    //for (int i = 0; i < n; ++i)
	//    printf("x[%d] = %f\n", i, solution[i]);

	tf = omp_get_wtime();

	//Check that solution is correct
	check_solution(m,b,solution,n);

    //free augmented matrix
    free_matrix(matrix,n);

    double time = tf-ts;
    return time;
}

int main(int argc, char const *argv[])
{
	time_t t; 
	//Intializes random number generator
	srand((unsigned) time(&t));
	//Iterate over matrices of size 2x2 to 20x20
	printf("Time register for different dimensions and different amount of threads:	\n");
	printf("Dimension(n)\tSerial\t2 threads\t3 threads\t4 threads\n");
	for (int n = 10; n <= 50; n++)
	{
		printf("%d\t",n);

		double ** matrix = generate_matrix(n);
		double * b = generate_vector(n);

		double serial_t = GE_solve(matrix,b, n);
	
		while(serial_t==-1)
		{
			free_matrix(matrix,n);
			free(b);
			matrix = generate_matrix(n);
			b = generate_vector(n);
			serial_t = GE_solve(matrix,b, n);
		}


		//Iterate with different number of threads
		printf("%f\t", serial_t);

		for (int nthreads = 2; nthreads <= 4; nthreads++)
		{
			double parallel_t = GE_solve_parallel(matrix,b, n, nthreads);
			printf("%f\t", nthreads, parallel_t );
		}

		free_matrix(matrix,n);
		free(b);
		printf("\n");
	}
}