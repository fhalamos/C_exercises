#include<stdio.h>
#include<stdlib.h>

double ***d3darr_contiguous(size_t l, size_t m, size_t n)
{

	/*Access to the l first pointers (1st dimension of the array). 
	We need to reserve l elements, each of them is a pointer to a double** */

	double ***lPointers= (double***) malloc(l*sizeof(double**));

	
	/*Second dimension of the array
	For each of de l elements, we need to save m elements
	Each element is of type double*
	*/
	double **mPointers = (double**) malloc(l*m*sizeof(double*));

	/*Third dimension array
	Long enough to save all the numbers*/
	double *nPointers = (double*) malloc(l*m*n*sizeof(double));


	/*No we have all the structure - memory allocations - for the array.
	We make the neccesary connections between the pointes so as to form the 3Darray*/

	int i;
	for(i=0; i<l;i++)
	{
		lPointers[i] = mPointers+ i*m;
		/*Equivalent to lPointers[i] = &mPointers[i*m];*/
	}


	int j;
	for(j=0; j<l*m;j++)
	{
		mPointers[j]= nPointers+j*n;
		/*Equivalent to	mPointers[j]= &nPointers[j*n];*/
	}
	
	return lPointers;
}

int main(int argc, char **argv){

	int i, j, k;
	int n = atoi(argv[1]);

	double ***matrix = d3darr_contiguous(n,n,n);



	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			for(k=0; k<n; k++){
				matrix[i][j][k] = n*n*i + n*j +k;
			}
		}
	}


	printf("Matrix with 1D indexing:\n");
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			for(k=0; k<n; k++){
				printf("%.1f ", matrix[i][j][k]);
			}
		printf("\n");
		}
	printf("\n");
	}

	printf("Matrix with 3D indexing:\n");
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			for(k=0; k<n; k++){
				printf("%.1f ", matrix[0][0][n*n*i + n*j + k]);
			}
		printf("\n");
		}
	printf("\n");
	}

	return 0;
}
