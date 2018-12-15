#include <stdio.h>
#include <stdlib.h>

/*Write a function called GE solve() that takes an n × n input matrix A and RHS vector b and
returns the vector x that satisfies Ax = b. Solve the system using Gaussian elimination with
back-substitution. You may assume that the linear system has a unique solution. Include a
simple test driver that initializes a small test matrix and RHS vector, calls GE solve(), and
prints the results.*/

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
			
			printf("Wrong solution. sum = %f, b[%d] = %f \n", sum, i ,b[i]);
			return 0;
		}
	}
	
	//printf("Solution checked and correct.\n");
	return 1;
}

double ** copy_matrix(double**m, int n)
{
	double ** copy= (double**) malloc(n*sizeof(double*));
	for (int i = 0; i < n; ++i)
		copy[i] = (double*) malloc((n+1)*sizeof(double));

	//Copy elements of m
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j <= n; ++j)
		{
			copy[i][j] = m[i][j];
		}
	}

	return copy;
}

//Solve the system using Gaussian elimination with back-substitution
int GE_solve(double ** m, double* b, int n)
{
	double solution[n];

	//Step 0: Generate the augmented matrix
	double ** matrix = generate_augmented_matrix(m,b,n);
	double ** matrix_original = copy_matrix(matrix,n);


	//Gaussian eliminarion
	//Traverse the matrix through columns.
	//In each iteration we will leave only zeros bellow pivot in column j
	//After finishing loop we will have an upper triangular matrix
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

	//Check that solution is correct (not infinit nor null solution)
	if(!check_solution(m,b,solution,n))
	{
	    free_matrix(matrix,n);
	    free_matrix(matrix_original,n);
		return 1;
	}

	//Print results
	printf("Original augmented matrix:\n");
    print_augmented_matrix(matrix_original,n);
    printf("\n");

	printf("Upper triangular augmented matrix:\n");
	print_augmented_matrix(matrix,n);
	printf("\n");

	printf("Solution:\n");
    for (int i = 0; i < n; ++i)
   		printf("x[%d] = %f\n", i, solution[i]);

    //free augmented matrix
    free_matrix(matrix,n);
    free_matrix(matrix_original,n);

    return 0;
}

int main(int argc, char const *argv[])
{
	//Intializes random number generator
	time_t t; 
	srand((unsigned) time(&t));

	int n = atoi(argv[1]);

	double ** matrix = generate_matrix(n);
	double * b = generate_vector(n);

	int error = GE_solve(matrix,b, n);
	while(error)
	{
		free_matrix(matrix,n);
		free(b);
		matrix = generate_matrix(n);
		b = generate_vector(n);
		error = GE_solve(matrix,b, n);
	}
}