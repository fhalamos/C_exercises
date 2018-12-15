#include <stdio.h>
#include <stdlib.h>

/*Write a function called GE solve() that takes an n × n input matrix A and RHS vector b and
returns the vector x that satisfies Ax = b. Solve the system using Gaussian elimination with
back-substitution. You may assume that the linear system has a unique solution. Include a
simple test driver that initializes a small test matrix and RHS vector, calls GE solve(), and
prints the results.*/

//Return nxn matrix
double ** generate_tridiagonal_matrix(int n)
{
	double ** matrix = (double**) malloc(n*sizeof(double*));
	for (int i = 0; i < n; ++i)
		matrix[i]= (double*)malloc(3*sizeof(double));//only save the numbers of the 3 diagonals

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			matrix[i][j]=rand()%10+1;
		}
	}
	matrix[0][0]=0;
	matrix[n-1][2]=0;
	return matrix;
}

double* generate_vector(int n)
{
	double* b = (double*) malloc(n*sizeof(double));
	for (int i = 0; i < n; ++i)
		b[i]=rand()%10+1;
	return b;
}

double* generate_vector_a(int n)
{
	double* b = (double*) malloc(n*sizeof(double));
	b[0]=-1;
	b[1]=4;
	b[2]=4;
	b[3]=8;
	return b;
}

double* generate_vector_b(int n)
{
	double* b = (double*) malloc(n*sizeof(double));
	b[0]=2;
	b[1]=3;
	b[2]=6;
	b[3]=1;
	return b;
}

double* generate_vector_c(int n)
{
	double* b = (double*) malloc(n*sizeof(double));
	b[0]=2;
	b[1]=7;
	b[2]=9;
	b[3]=-1;
	return b;
}

double* generate_vector_d(int n)
{
	double* b = (double*) malloc(n*sizeof(double));
	b[0]=7;
	b[1]=8;
	b[2]=4;
	b[3]=3;
	return b;
}

void free_matrix(double ** m, int rows)
{
	for (int i = 0; i < rows; ++i)
		free(m[i]);
	free(m);
}

int check_solution(double ** m, double* b, double* x, int n)
{
	double a[n];
//	double b[n];
	double c[n];

	for (int i = 0; i < n; ++i)
	{
		/* code */
	}

	for (int i = 0; i < n; ++i)
	{
		double sum=0;
		for (int j = 0; j < 3; ++j)
		{
			sum+=m[i][j]*x[i+j];
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


double ** copy_tridiagonal_matrix(double** m, int n)
{
	double ** copy = (double**) malloc(n*sizeof(double*));
	for (int i = 0; i < n; ++i)
		copy[i]= (double*)malloc(3*sizeof(double));//only save the numbers of the 3 diagonals

	double a=rand()%10+1;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			copy[i][j]=m[i][j];
		}
	}
	return copy;
}

double * copy_vector(double *d, int n)
{
	double* copy = (double*) malloc(n*sizeof(double));
	for (int i = 0; i < n; ++i)
		copy[i]=d[i];
	return copy;
}


void print_tridiagonal_matrix(double *a, double *b, double *c, int n)
{
	printf("%.2f\t%.2f", b[0],c[0]);
	for (int i = 0; i <= n-2; ++i)
		printf("0\t");
	printf("\n");

	for (int i = 1; i < n-1; ++i)
	{
		for (int j = 0; j < i-1; ++j)
			printf("0\t");
		printf("%.2f\t%.2f\t%.2f", a[i],b[i],c[i]);
		for (int j = 0; j < n-i-1	; ++j)
			printf("0\t");
		printf("\n");
	}

	for (int i = 0; i < n-2; ++i)
		printf("0\t");
	printf("%.2f\t%.2f\n", a[n-1],b[n-1]);
	
}

void print_vector(double* d, int n)
{
	for (int i = 0; i < n; ++i)
		printf("%f\t", d[i]);
	printf("\n");
}


int tri_solve(double* a, double* b, double* c, double * d, int n) {
    

	double x[n];

    double * a_o = copy_vector(a,n);
    double * b_o = copy_vector(b,n);
    double * c_o = copy_vector(c,n);
    double * d_o = copy_vector(d,n); 

    printf("Printing matrix...\n");
    print_tridiagonal_matrix(a,b,c,n);
    printf("\n");
    printf("Vector b: \n");
    print_vector(d,n);

    //Move down
    for (int i = 1; i < n; ++i)
    {
    	double factor = a[i]/b[i-1];

    	a[i] = a[i]-b[i-1]*factor;
		b[i] = b[i]-c[i-1]*factor;    	
    	
    	d[i] = d[i]-d[i-1]*factor;
    }


    printf("\n");

    //Move up
	x[n-1]=d[n-1]/b[n-1];
    for (int i = n-2; i >= 0; i--)
    {
    	x[i] = (d[i]-c[i]*x[i+1])/b[i];
    }

    
    printf("Solution\n");
	for (int i = 0; i < n; ++i)
	{
		printf("x[%d] = %f\n", i, x[i]);
	}

	/*
	printf("\n");
    printf("Vector d\n");
    print_vector(x_o,n);
    printf("\n");

	//Check that solution is correct (not infinit nor null solution)
	/*	
	if(!check_solution(m,d_original,d,n))
	{
		printf("problema\n");
		return 1;
	}*/
}

//Same sintaxis as https://es.wikipedia.org/wiki/Algoritmo_para_matrices_tridiagonales


int tri_solve2(double ** m, double* d, int n)
{
	double ** matrix_original = copy_tridiagonal_matrix(m,n);
	double * d_original = copy_vector(d,n);

	//print_tridiagonal_matrix(m,n);
	printf("\n");

	print_vector(d,n);
	
	double x[n];

	double a_i, b_i, c_i, d_i, c_i_p, prev_c_i_p, d_i_p, prev_d_i_p;

	for (int i = 0; i < n; ++i)
	{
		if(i==0)
		{
			b_i = m[0][0];
			c_i = m[0][1];
			d_i = d[0];

			c_i_p = c_i/b_i;
			m[0][1] = c_i_p;			
			prev_c_i_p = c_i_p;

			d_i_p= d_i/b_i;
			d[0] = d_i_p;
			prev_d_i_p = d_i_p;
		}
		else
		{
			a_i = m[i][i-1];
			b_i = m[i][i];
			c_i = m[i][i+1];
			d_i = d[i];

			if(i!=n-1)
			{
				c_i_p = c_i / (b_i - (prev_c_i_p*a_i));
				m[i][i+1] = c_i_p;
				prev_c_i_p = c_i_p;
			}

			d_i_p= (d_i-(prev_d_i_p*a_i))/(b_i-(prev_c_i_p*a_i));
			d[i] = d_i_p;
			prev_d_i_p = d_i_p;
		}
	}

	x[n-1] = d[n-1];
	for (int i = n-2; i >= 0; i--)
		x[i]=d[i]-m[i][i+1]*x[i+1];
	
	
	for (int i = 0; i < n; ++i)
	{
		printf("x[%d]%f\n", i, x[i]);
	}

	//Check that solution is correct (not infinit nor null solution)
	
	if(!check_solution(matrix_original,d_original,x,n))
	{
		printf("problema\n");
	    free_matrix(matrix_original,n);
	    printf("a\n");
		return 1;
	}
	/*
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
	*/
    return 0;
}



/*Devise an O(n) algorithm to solve Ax = b using elimination and backsubstitution assuming that
A is tridiagonal. Implement this in a function called tri solve(). Use a data structure for A
that does not require storage of the zeros that are not part of the diagonal, upper diagonal, or
lower diagonal. Include a simple test driver that initializes a small tridiagonal test */

int main(int argc, char const *argv[])
{
	//Intializes random number generator
	time_t t; 
	srand((unsigned) time(&t));

	int n = atoi(argv[1]);


	//We will only save the elements of the diagonals
	double * a = generate_vector(n);
	double * b = generate_vector(n);
	double * c = generate_vector(n);
	
	double * d = generate_vector(n);//generate_vector(n);

	int error = tri_solve(a,b,c,d,n);
	
	/*
	while(error)
	{
		printf("holaaaa\n");
		//free_matrix(matrix,n);
		printf("aaa\n");
		free(b);
		matrix = generate_tridiagonal_matrix(n);
		b = generate_vector(n);
		error = tri_solve(matrix,b, n);
	}
	*/
}