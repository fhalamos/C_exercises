#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINES 1000
#define MAX_CHAR_PER_LINE 1000

int read_file(char *filename, char text[][MAX_CHAR_PER_LINE])
{
	/*Reference: https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm*/
	FILE *fp;
	int linesCounter=0;

	/* opening file for reading */
	fp = fopen(filename , "r");

	while(fgets (text[linesCounter], MAX_CHAR_PER_LINE, fp)!=NULL )
	{
		/*printf("%s", text[linesCounter]);*/
		linesCounter++;
		
		if (linesCounter >= MAX_LINES)
		{
            break;
        }
	}
	/*printf("\n");*/
	fclose(fp);

	return linesCounter;
}


/* qsort C-string comparison function */ 
int cstring_cmp(const void *a, const void *b) 
{ 
    

    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    /*Ill use strcmp fuction to compare string*/

    return strcmp(*ia, *ib);

} 





void alphabetize(char **line_pntrs,int nlines)
{
	/*
	printf("Before sorting the list is: \n");
	int n;
	for( n = 0 ; n < nlines; n++ ) {
	  printf("%s", line_pntrs[n]);
	}
	printf("\n%d\n", nlines);
	*/

	qsort(line_pntrs, nlines, sizeof(char*), cstring_cmp);

	/*
	printf("\nAfter sorting the list is: \n");
	for( n = 0 ; n < nlines; n++ ) {   
	  printf("%s", line_pntrs[n]);
	}
	printf("Done!");
	*/
  
}

void printFile(char in[][MAX_CHAR_PER_LINE], int n)
{

	int i=0,j=0;
	
	/* Test the character array */
	for( i = 0; i < n; )
	{
        putchar(in[i][j]);
		if( in[i][j] != '\n')
			j++;
		else
		{
			i++;
			j=0;
		}
	}


}

int main(int argc, char **argv){

	char **line_pntrs;
	char in[MAX_LINES][MAX_CHAR_PER_LINE];
	char *filename = argv[1];
	int i=0,j=0, n;
	char c;

	n = read_file(filename,in);


	/*printFile(in, n);*/




	/* Array of pointers to lines */
	line_pntrs = (char **) malloc(MAX_LINES*sizeof(char *));

	for( i = 0; i < MAX_LINES; i++ )
		line_pntrs[i] = in[i];


	alphabetize(line_pntrs,n);

	for(i=0;i<n;i++)
		printf("%s", line_pntrs[i]);

	return 0;
}

