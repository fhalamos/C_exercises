#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define WORD_LEN 64
#define DEF_LEN 1024
#define COMMAND_LEN 32 

//Max length of all words in dictionary
#define MAX_WORD_LENGTH 20

int equal_words(char *str1, char* str2)
{
	if(strlen(str1)== strlen(str2) && strncmp(str1,str2,strlen(str2))==0)
		return 1;
	else
		return 0; 
}

typedef struct node{
	char word[WORD_LEN+1];
	char definition[DEF_LEN+1];
	struct node * next;
	int bin;
} Node;

typedef struct hashmap{
	Node ** data; 
	int n_bins;
	int n_entries;

    unsigned int (*hash_algorithm)(char*, int, int*); 

	//vector of random integer values, defines each specific hash function
    int * a;

    int max_entries_in_bin;

  
} Hashmap;

/* Function that checks whether or not a given number is
 * a prime number or not. */
int is_prime(int input){
    int i;
 
    int prime = 1;
 
    if(input == 2){
        return 1;
    }
 
    if(input%2 == 0 || input <= 1){
        prime = 0;
    } else {
        for(i=3; i<=sqrt(input); i+=2){
            if(input%i == 0){
                prime = 0;
            }
        }
    }
    return prime;
}

/* Function for determining the next prime number */
int next_prime_number(int i){
     
    int nextPrimeNumber;
 
    nextPrimeNumber = i + 1;

    /* if the number is even, make it odd (2 is special case) */
    if(nextPrimeNumber%2 == 0 && nextPrimeNumber != 2){ 
        nextPrimeNumber+=1;
    }

    /* while its not a prime number, check the next odd number */
    while(!is_prime(nextPrimeNumber)){
        nextPrimeNumber+=2;
    }

    return nextPrimeNumber;
}
 

int dot_product(int *a, int *b, int r)
{
	int sum=0;
	for (int i = 0; i < r; ++i)
		sum+=a[i]*b[i];
	return sum;
}


void fill_array_a(Hashmap* dictionary)
{
	time_t t;
	srand((unsigned) time(&t));

	//Fill up the random integers array a
    for (int i = 0; i < MAX_WORD_LENGTH; ++i)
    	dictionary->a[i] = 	rand() % dictionary->n_bins;
}

//int max=0;
unsigned int naive_hash(char * word, int nbins, int* a)
{
	unsigned int h = 0;
	int c;

	//In case we want to calculate the max index, uncomment code

	//char * w_copy = malloc(strlen(word) + 1); 
	//strcpy(w_copy, word);

	while(c = *word++)
		h += c;

	/*
	if(h>max)
	{
		max=h;
		printf("new max, word %s, max %d\n", w_copy, max);
	}
	*/
	//free(w_copy);

	return h % nbins;
}

unsigned int bernstein_hash(char * word, int nbins, int* a)
{
	unsigned int h = 5381;
	int c;
	while(c = *word++)
		h = 33 * h + c;
	return h % nbins;
}

unsigned int FNV_hash(char * word, int nbins, int* a)
{
	unsigned long h = 14695981039346656037lu;
	char c;
	while(c = *word++)
	{
		h = h * 1099511628211lu;
		h = h ^ c;
	}
	return h % nbins;
}



unsigned int universal_hash(char * word, int n_bins, int* a)
{
	//k is the key vector, of length r, with each entry containing an
	//integer value ki < s.
	int * k = (int *) malloc(MAX_WORD_LENGTH*sizeof(int));
	for (int i = 0; i < MAX_WORD_LENGTH; ++i)
	{
		if(i < strlen(word))
			k[i] = (int)word[i]%n_bins;
		else
			k[i] = 0;
	}

	int dot_prod_mod = dot_product(a,k,MAX_WORD_LENGTH) % n_bins;
	free(k);
	return dot_prod_mod;
}

int find_word(Hashmap* dictionary, char * word)
{
	unsigned int index = (*(dictionary->hash_algorithm))(word, dictionary->n_bins, dictionary->a); 

	Node* n = dictionary->data[index];

	while(n && !equal_words(n->word, word))
		n = n->next;

	if(n && equal_words(n->word,word))
	{
		printf("%s: %s\n", n->word, n->definition );
		return 0;
	}

	else
		return 1;
}

int delete_word(Hashmap* dictionary, char * word)
{
	unsigned int index = (*(dictionary->hash_algorithm))(word, dictionary->n_bins, dictionary->a); 

	Node* n = dictionary->data[index];

	//In case the word is in the front of the linked list
	if(n && equal_words(n->word,word))
	{
		dictionary->data[index] = n->next;
		free(n);
		dictionary->n_entries--;
		return 0;
	}

	//Look in the linked list
	Node * parent;
	while(n && !equal_words(n->word, word))
	{	
		parent = n;
		n = n->next;
	}

	if(n && equal_words(n->word, word))
	{
		parent->next = n->next;
		free(n);
		dictionary->n_entries--;
		return 0;
	}
	else
		return 1;
}


void clear_dictionary(Hashmap * dictionary)
{
	for(int i=0; i<dictionary->n_bins;i++)
	{
		Node* n = dictionary->data[i];
		while(n!=NULL)
		{
			Node * to_be_deleted = n;
			n = n->next;
			free(to_be_deleted); 
		}
		dictionary->data[i] = NULL;
	}
	dictionary = (Hashmap*) realloc(dictionary, 16);
	dictionary->n_entries=0;
	dictionary->n_bins=16;
}

int comparator(const void *p, const void *q) 
{
	Node *n1 = *(Node**) p;
	Node *n2 = *(Node**) q;
	return strcmp(n1->word,n2->word);
}  


void print_dictionary(Hashmap* dictionary)
{
	//printf("n_bins: %d\n", dictionary->n_bins);
	//printf("n_entries: %d\n", dictionary->n_entries);

	Node ** allNodes = (Node**) malloc(dictionary->n_entries*sizeof(Node*));
	int counter=0;



	for(int i =0; i<dictionary->n_bins;i++)
	{
		Node * n = dictionary->data[i];
		while(n!=NULL)
		{
			allNodes[counter]=n;
			counter++;
			n = n->next;
		}
	}


	qsort(	allNodes,
			dictionary->n_entries,
			sizeof(Node*), 
			comparator);

	for(int i=0; i<dictionary->n_entries;i++)
		printf("[%d] %s: %s\n", allNodes[i]->bin, allNodes[i]->word, allNodes[i]->definition);

	free(allNodes);	
}

//Used to print empty bins too and see distribution of words in bins.
//not orderes alphabetically but by bin number
void print_dictionary_with_empty_bins(Hashmap* dictionary)
{
	printf("bins: %d\n", dictionary->n_bins);
	printf("entries: %d\n", dictionary->n_entries);

	for(int i =0; i<dictionary->n_bins;i++)
	{
		Node * n = dictionary->data[i];
		if(n)
		{
			while(n!=NULL)
			{
				printf("[%d] %s: %s\n", n->bin, n->word, n->definition);
				n = n->next;
			}
		}
		else
			printf("[%d] %s\n", i, "EMPTY");
	}
}

Node* create_node(char * word, char* definition, int index)
{
	Node* new_node = (Node*) malloc(sizeof(Node));
	memcpy(new_node->word, word, strlen(word)+1);
	memcpy(new_node->definition, definition, strlen(definition)+1);
	new_node->bin = index;
	new_node->next = NULL;
	return new_node;
}

void rehash_all_elements(Hashmap* dictionary)
{
	//Save elements in temp array
	Node ** tmp_list_of_nodes = (Node**) malloc(dictionary->n_entries*sizeof(Node*));
	int counter=0;
	for(int i =0; i<dictionary->n_bins;i++)
	{
		Node * n = dictionary->data[i];
		if(n!=NULL)
		{
			while(n!=NULL)
			{
				tmp_list_of_nodes[counter] = n;
				counter++;
				n = n->next;
			}
			//Erase old hashes	
			dictionary->data[i] = NULL;
		}
	
	}
	//Rehash elements
	int n_entries = dictionary->n_entries;
	dictionary->n_entries=0;
	dictionary->max_entries_in_bin=0;
	for (int i=0; i<n_entries;i++)
		add_word(dictionary,tmp_list_of_nodes[i]->word, tmp_list_of_nodes[i]->definition);

	free(tmp_list_of_nodes);
}


int add_word(Hashmap *dictionary, char * word, char * definition)
{
	//If load_factor>0.75 double bins in dictionary and rehash all elements	
	double load_factor;
	load_factor = (double)(dictionary->n_entries+1)/dictionary->n_bins;
	if(load_factor>=0.75)
	{

		int old_size = dictionary->n_bins;
		int new_size;
		if(dictionary->hash_algorithm!=&universal_hash)		
			new_size = dictionary->n_bins*2;
		else //In case we are in universal hashing
			new_size = next_prime_number(dictionary->n_bins*2);

		//Create new space
		dictionary->data = (Node**)realloc(dictionary->data, new_size*sizeof(Node*));	
		dictionary->n_bins = new_size;
		for(int i=old_size; i<new_size; i++)
			dictionary->data[i]=NULL;

		//In case of universal hash, generate new array a
		if(dictionary->hash_algorithm==&universal_hash)	
			fill_array_a(dictionary);

		rehash_all_elements(dictionary);
	}


	unsigned int index = (*(dictionary->hash_algorithm))(word, dictionary->n_bins, dictionary->a); 

	//If nothing in the destiny bucket, save there
	if(dictionary->data[index]==NULL)
	{
		Node* new_node = create_node(word,definition,index);
		dictionary->data[index] = new_node;
		dictionary->n_entries++;

		if(dictionary->max_entries_in_bin==0)
			dictionary->max_entries_in_bin=1;
	}

	//If word is already in bucket
	else if(equal_words(dictionary->data[index]->word,word))
	{
		printf("Duplicate Key\n");
		return 1;
	}

	else 
	{
		Node* last_node = dictionary->data[index];
		int entries_in_bin=1;
		while(last_node->next!=NULL)
		{
			entries_in_bin++;
			last_node = last_node->next;

			if(last_node && equal_words(last_node->word,word))
			{
				printf("Duplicate Key\n");
				return 1;
			}
		}
		
		Node * new_node= create_node(word, definition, index);
		last_node->next = new_node;
		entries_in_bin++;
		dictionary->n_entries++;


		if(	dictionary->hash_algorithm==&universal_hash &&
			dictionary->max_entries_in_bin < entries_in_bin)
			
			dictionary->max_entries_in_bin = entries_in_bin;
	}



	if(	dictionary->max_entries_in_bin > dictionary->n_entries*0.25 &&
		dictionary->n_entries > 10)
	{
		fill_array_a(dictionary);
		rehash_all_elements(dictionary);
	}

	//get_stats(dictionary);

	return 0;

}



int import_file(char * fname, Hashmap * dictionary )
{
    FILE* file = fopen(fname, "r"); /* should check the result */
    char line[WORD_LEN + 3 + DEF_LEN];

    if(file==NULL)
    	return 1;

    while (fgets(line, sizeof(line), file)) {
        
        if(line[strlen(line)-1]=='\n')
        	line[strlen(line)-1] = '\0';

    	char *word = strtok(line, " ");
    	char *definition= strtok(NULL, "");
  
    	add_word(dictionary, word, definition);

    }
    fclose(file);
    return 0;
}

double calculate_bin_fraction(Hashmap* dictionary)
{
	int counter_bins_used =0;
	for(int i=0; i<dictionary->n_bins; i++)
	{
		if(dictionary->data[i])
			counter_bins_used++;
		
	}
	return (double)counter_bins_used/dictionary->n_bins;
}

int calculate_max_entries(Hashmap* dictionary)
{
	int max_entries =0;
	for(int i=0; i<dictionary->n_bins; i++)
	{
		int n_elements_in_current_bin=0;
		Node * n = dictionary->data[i];
		while(n)
		{
			n_elements_in_current_bin++;	
			n = n->next;
		}
		if(n_elements_in_current_bin>max_entries)
			max_entries = n_elements_in_current_bin;
	}
	return max_entries;
}

int get_stats(Hashmap* dictionary)
{
	int bins = dictionary->n_bins;
	double occupancy = (double)dictionary->n_entries/dictionary->n_bins;
	double used_bin_fraction = calculate_bin_fraction(dictionary);
	int max_entries = calculate_max_entries(dictionary);
	printf("Bins: %d, Occupancy: %f, Used bin fraction: %f, Max entries in a bin: %d\n",
			bins, occupancy, used_bin_fraction, max_entries);
	/*In case we want less verbose output:
	printf("%d,%f,%f,%d\n",
			bins, occupancy, used_bin_fraction, max_entries);*/
	return 0;
}



Hashmap* create_empty_dictionary(unsigned int (*hash_algorithm)(char*, int, int*), int n_bins)
{
	Hashmap * dictionary = (Hashmap*)malloc(sizeof(Hashmap));
	dictionary->data = (Node**)malloc(n_bins*sizeof(Node*));
	for(int i=0; i<n_bins; i++)
		dictionary->data[i]=NULL;
	dictionary->n_bins = n_bins;

	dictionary->n_entries = 0;
	dictionary->max_entries_in_bin=0;

	dictionary->hash_algorithm = hash_algorithm;

	if(hash_algorithm == &universal_hash)
	{
	    dictionary->a = (int *) malloc(MAX_WORD_LENGTH*sizeof(int));
    	fill_array_a(dictionary);
	}
	
	return dictionary;
}

void print_stats_for_different_dictionaries_and_hashes()
{
	Hashmap * dictionary;

	
	printf("naive_hash\n");
	dictionary = create_empty_dictionary(&naive_hash, 16);	

	printf("With small dictionary\n");
	import_file("small_dictionary.txt", dictionary);
	get_stats(dictionary);
	clear_dictionary(dictionary); 

	printf("With medium dictionary\n");
	import_file("medium_dictionary.txt", dictionary);
	get_stats(dictionary);
	clear_dictionary(dictionary);
	
	printf("With large dictionary\n");
	import_file("large_dictionary.txt", dictionary);
	get_stats(dictionary);
	clear_dictionary(dictionary);

	free(dictionary);
	printf("\n");

	printf("bernstein_hash\n");
	dictionary = create_empty_dictionary(&bernstein_hash, 16);
	
	printf("With small dictionary\n");
	import_file("small_dictionary.txt", dictionary);
	get_stats(dictionary);
	clear_dictionary(dictionary);

	printf("With medium dictionary\n");
	import_file("medium_dictionary.txt", dictionary);
	get_stats(dictionary);
	clear_dictionary(dictionary);
	
	printf("With large dictionary\n");
	import_file("large_dictionary.txt", dictionary);
	get_stats(dictionary);
	clear_dictionary(dictionary);

	free(dictionary);	
	printf("\n");

	printf("FNV_hash\n");
	dictionary = create_empty_dictionary(&FNV_hash, 16);

	printf("With small dictionary\n");
	import_file("small_dictionary.txt", dictionary);
	get_stats(dictionary);
	clear_dictionary(dictionary);
	
	printf("With medium dictionary\n");
	import_file("medium_dictionary.txt", dictionary);
	get_stats(dictionary);
	clear_dictionary(dictionary);
	
	printf("With large dictionary\n");
	import_file("large_dictionary.txt", dictionary);
	get_stats(dictionary);
	clear_dictionary(dictionary);
	
	free(dictionary);
}




int main(int argc, char **argv)
{
	int algorithm = atoi(argv[1]);
	Hashmap * dictionary; 
	
	if(algorithm==1)
		dictionary = create_empty_dictionary(&naive_hash, 16);
	else if (algorithm==2)
		dictionary = create_empty_dictionary(&bernstein_hash, 16);
	else if (algorithm==3)
		dictionary = create_empty_dictionary(&FNV_hash, 16);
	else
		dictionary = create_empty_dictionary(&universal_hash, next_prime_number(MAX_WORD_LENGTH));


	while(1)
	{
		int error;
		char command[COMMAND_LEN];

		/* Get the User's Command */
		printf(">$ ");
		scanf("%s", command);

		/* Add word */
		if( strncmp(command, "add", COMMAND_LEN) == 0 )
		{
			char word[WORD_LEN];
			char definition[DEF_LEN];
			char * pos;
			scanf("%s ",word);
			/* Gets the rest of the input */
			fgets(definition, DEF_LEN, stdin);
			if ((pos=strchr(definition, '\n')) != NULL)
				*pos = '\0';

			/* Adds to dictionary */
			error = add_word(dictionary, word, definition);

			if( error == 0 )
				printf("The word \"%s\" has been added successfully.\n", word);
			else
				printf("Error when adding word \"%s\".\n", word);

		}
		/* Import file */
		if( strncmp(command, "import", COMMAND_LEN) == 0 )
		{
			char fname[DEF_LEN];
			scanf("%s",fname);
			error = import_file(fname, dictionary );
			if( error == 0 )
				printf("The file \"%s\" has been imported successfully.\n", fname);
			else
				printf("Error when adding file \"%s\".\n", fname);
		}
		/* Delete word */
		if( strncmp(command, "delete", COMMAND_LEN) == 0 )
		{
			char word[WORD_LEN];
			scanf("%s",word);
			error = delete_word(dictionary, word);
			if( error == 0 )
				printf("The word \"%s\" has been deleted successfully.\n", word);
			else
				printf("Error when deleting word \"%s\".\n", word);
		}
		/* Find word */
		if( strncmp(command, "find", COMMAND_LEN) == 0 )
		{
			char word[WORD_LEN];
			scanf("%s",word);
			error = find_word(dictionary, word);
			if( error == 1 )
				printf("Error when finding word \"%s\".\n", word);
		}
		/* Print dictionary */
		if( strncmp(command, "print", COMMAND_LEN) == 0 )
			print_dictionary(dictionary);
		/* clear dictionary */
		if( strncmp(command, "clear", COMMAND_LEN) == 0 )
			clear_dictionary(dictionary);
		/* quit program */
		if( strncmp(command, "quit", COMMAND_LEN) == 0 )
			break;

		/* Get stats*/
		if( strncmp(command, "stats", COMMAND_LEN) == 0 )
		{
			error = get_stats(dictionary);
			if(error)
				printf("Error when calculating stats");
		}

		/* Compare hash functions*/
		if( strncmp(command, "compare", COMMAND_LEN) == 0 )
			print_stats_for_different_dictionaries_and_hashes();
		
		/* Print with empty bins*/
		if( strncmp(command, "print2", COMMAND_LEN) == 0 )
			print_dictionary_with_empty_bins(dictionary);
		
	}

	return 0;
}
