#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_CHAR_PER_LINE 1000

/* Returns number of lines read from file */
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

int main(int argc, char **argv){

	char *filename = argv[1];
	char text[MAX_LINES][MAX_CHAR_PER_LINE];
	int i=0,j=0;
	int nlines;

	nlines = read_file(filename,text);
	
	/* Test the character array */
	for( i = 0; i < nlines; )
	{
        putchar(text[i][j]);
		if( text[i][j] != '\n')
			j++;
		else
		{
			i++;
			j=0;
		}
	}

	return 0;
}
