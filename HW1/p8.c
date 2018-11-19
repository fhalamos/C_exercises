#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_WORDS 1000
#define MAX_CHAR_PER_WORD 25


int hasRepeatedChar(char * c)
{
    int i, j;
    for (i = 0; i < strlen(c)-1; i++)
    {       
        if (c[i] == c[i+1])
        { 
            return 1; 
        }  
    }   
    return -1; 
}

void increaseCounter(char ** listWords, int *counter, int n, char* word)
{
	int i;
	for(i=0;i<n;i++)
	{
		if(strcmp(listWords[i], word)==0)
		{
			counter[i]++;
		}
	}
}



int inList(char ** listWords, int n, char* word)
{
	int i;
	for(i=0;i<n;i++)
	{
		if(strcmp(listWords[i], word)==0)
			return 1;
	}
	return 0;
}

char* remove_punct_and_make_lower_case(char* str)
{
	/*Reference: https://stackoverflow.com/questions/1841758/how-to-remove-punctuation-from-a-string-in-c*/
	char* new_string = (char *) malloc(strlen(str));

    int len = strlen(str);

    int i;
    int p;
    for (i = 0; i < len; i++)
    {
    	if (! ispunct((unsigned char)str[i]))
        {
            new_string[p] = tolower((unsigned char)str[i]);
            p++;
        }

    }

    return new_string;
}


int readFile(char ** words_pntrs, char* filename)
{

	FILE *file;
	file = fopen(filename , "r");

	char current_word[MAX_CHAR_PER_WORD];
	int words_counter=0;

	while (fscanf(file, "%s", current_word) == 1)
	{
		char * word = (char*) malloc(MAX_CHAR_PER_WORD*sizeof(char));
		memcpy(word,current_word,strlen(current_word));
		
		/*Program should be insensitive to
		punctuation marks and capitalization.
		Hence, we save words without punctuation and in lower case*/
		word = remove_punct_and_make_lower_case(word);

		words_pntrs[words_counter] = word;
		words_counter++;
	}
	
	return words_counter;

}

void print_results(char ** listWords, int *counterWords, int n)
{
	int i;
	for(i=0; i<n;i++)
	{
		printf("%s : %d\n",listWords[i], counterWords[i]);

	}

}

void add_word_to_list(char ** list_words, int *counter_words, int n_repeated_words, char *word)
{
	list_words[n_repeated_words]=word;
	counter_words[n_repeated_words]=1;
}


int main(int argc, char **argv)
{
	/* Read in words */
	char **words_pntrs = (char **) malloc(MAX_WORDS*sizeof(char *));
	int words_counter=0;
	words_counter = readFile(words_pntrs, argv[1]);

	/* Check for repeats & print results */

	char **words_repeated_ch_pntrs = (char **) malloc(MAX_WORDS*sizeof(char *));
	int *counter_of_words_w_repeated_ch = (int*)malloc(MAX_WORDS*sizeof(int));
	int n_repeated_words=0;

	int i;
	for(i=0; i<words_counter;i++)
	{
		if(hasRepeatedChar(words_pntrs[i])==1)
		{
			/*If we havent found it before, we add it to the list*/
			if(inList(words_repeated_ch_pntrs,n_repeated_words,words_pntrs[i])==0)
			{
				add_word_to_list(words_repeated_ch_pntrs,
							counter_of_words_w_repeated_ch,
							n_repeated_words,
							words_pntrs[i]);

				n_repeated_words++;
			}
			else
			{
				increaseCounter(words_repeated_ch_pntrs,
								counter_of_words_w_repeated_ch,
								n_repeated_words,
								words_pntrs[i]);
			}
		}
	}

	print_results(words_repeated_ch_pntrs,
				counter_of_words_w_repeated_ch,
				n_repeated_words);
	
	return 0;
}
