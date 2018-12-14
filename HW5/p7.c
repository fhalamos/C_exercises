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

	time_t t; 
	//Intializes random number generator
	srand((unsigned) time(&t));
	double a=rand()%10;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			matrix[i][j]=a;
			a=rand()%10;//(i+j)+6*i+1+i%2;
		}
	}
	return matrix;
}

double* generate_vector(int n)
{
	double* b = (double*) malloc(n*sizeof(double));
	for (int i = 0; i < n; ++i)
		b[i]=rand()%10;
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
//Solve the system using Gaussian elimination with back-substitution
void GE_solve(double ** m, double* b, int n)
{
	double solution[n];

	//Step 0: Generate the augmented matrix
	double ** matrix = generate_augmented_matrix(m,b,n);
	printf("Original augmented matrix:\n");
    print_augmented_matrix(matrix,n);
    printf("\n");

	//Gaussian eliminarion
	//Traverse the matrix through columns.
	//In each iteration we will leave only zeros bellow pivot in column j
	//After finishing loop we will have an upper triangular matrix
    double c;
    for(int j=0; j<n; j++) 
    {
        for(int i=0; i<n; i++)
        {
            if(i>j)
            {
                //c is the amount of times we need to substract row j to tow i
                c=matrix[i][j]/matrix[j][j];
                for(int k=0; k<=n; k++)
                {
                    matrix[i][k]=matrix[i][k]-c*matrix[j][k];
                }
            }
        }
    }
	printf("Upper triangular augmented matrix:\n");
	print_augmented_matrix(matrix,n);
	printf("\n");

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
    for (int i = 0; i < n; ++i)
	    printf("x[%d] = %f\n", i, solution[i]);

    //free augmented matrix
    free_matrix(matrix,n);

}

int main(int argc, char const *argv[])
{
	int n = atoi(argv[1]);


	double ** matrix = generate_matrix(n);

	double * b = generate_vector(n);


	GE_solve(matrix,b, n);

}