#include<stdio.h>
#include<stdlib.h>


/*Write a function that returns a pointer to a pointer to a double where
 the pointer points to a valid address
of m pointers-to-double each of which points to a valid
address of n doubles. You should implement this
function using m + 1 calls to malloc().*/

double **dmatrix_non_contiguous(size_t m, size_t n)
{
	/*ptList points to the beggining of a list of m elements,
	Each of those elements has to be big enough to save
	a pointer to a list of doubles*/
	double ** ptList;
	ptList = (double **) malloc(m*sizeof(double*));
	int i;
	/*For each of the m pointers*/
	for (i=0;i<m;i++)
		ptList[i] = (double*) malloc(n*sizeof(double));

	return ptList;

}

int main(int argc,char **argv){

	size_t m=0,n=0,i,j;
	double **matrix; 

	n = atoi(argv[1]);
	m = n;
	matrix = dmatrix_non_contiguous(n,n);


	/*The answer was already here...*/
	/* Test fill */
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			matrix[i][j] = n*i+j;
		}
	}

	/* Print matrix */
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			printf("%.1f ", matrix[i][j]);
		}
		putchar('\n');
	}
	return 0;
}

