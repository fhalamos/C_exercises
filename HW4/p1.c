#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define WORD_LEN 64
#define DEF_LEN 1024
#define COMMAND_LEN 32 

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

    unsigned int (*hash_algorithm)(char*, int); 
  
} Hashmap;

unsigned int naive_hash(char * word, int nbins)
{
	printf("using naive_hash\n");
	unsigned int h = 0;
	int c;
	while(c = *word++)
		h += c;
	return h % nbins;
}

unsigned int bernstein_hash(char * word, int nbins)
{
	printf("using bernstein_hash\n");
	unsigned int h = 5381;
	int c;
	while(c = *word++)
		h = 33 * h + c;
	return h % nbins;
}

unsigned int FNV_hash(char * word, int nbins)
{
	printf("using fnv\n");
	unsigned long h = 14695981039346656037lu;
	char c;
	while(c = *word++)
	{
		h = h * 1099511628211lu;
		h = h ^ c;
	}
	return h % nbins;
}


int find_word(Hashmap* dictionary, char * word)
{
	unsigned int index = (*(dictionary->hash_algorithm))(word, dictionary->n_bins); 

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
	unsigned int index = (*(dictionary->hash_algorithm))(word, dictionary->n_bins); 

	Node* n = dictionary->data[index];

	//In case the word is in the front of the linked list
	if(n && equal_words(n->word,word))
	{
		dictionary->data[index] = n->next;
		free(n);
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
	dictionary->n_entries=0;
}


int import_file(char * fname, Hashmap * dictionary )
{
    printf("a importar\n");
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

int comparator(const void *p, const void *q) 
{
	Node n1 = *(Node*) p;
	Node n2 = *(Node*) q;


	return strcmp(n1.word,n2.word);
}  


void print_dictionary(Hashmap* dictionary)
{
	printf("bins: %d\n", dictionary->n_bins);
	printf("entries: %d\n", dictionary->n_entries);

	Node allNodes[dictionary->n_entries];
	int counter=0;

	for(int i =0; i<dictionary->n_bins;i++)
	{
		Node * n = dictionary->data[i];
		while(n!=NULL)
		{
			allNodes[counter]=*n;
			counter++;
			n = n->next;
		}
	}

	qsort(	allNodes,
			dictionary->n_entries,
			sizeof(Node), 
			comparator);

	for(int i=0; i<dictionary->n_entries;i++)
		printf("[%d] %s: %s\n", allNodes[i].word, allNodes[i].definition);
}

Node* create_node(char * word, char* definition, int index)
{
	Node* new_node = (Node*) malloc(sizeof(Node));
	memcpy(new_node->word, word, strlen(word)+1);
	memcpy(new_node->definition, definition, strlen(definition)+1);
	new_node->next = NULL;
	new_node->bin = index;
}

int add_word(Hashmap *dictionary, char * word, char * definition)
{
	//If, when adding new word, load factor >= 0.75, resize hashmap
	double load_factor;
	load_factor = (double)(dictionary->n_entries+1)/dictionary->n_bins;
	
	//If condition, double bins in dictionary and rehash all elements	
	if(load_factor>=0.75)
	{
		//Double number of bins
		dictionary->data = (Node**)realloc(dictionary->data, dictionary->n_bins*2*sizeof(Node*));	
		dictionary->n_bins =dictionary->n_bins*2;

		//Save elements in temp array
		Node* list_of_nodes[dictionary->n_entries];
		int counter=0;

		for(int i =0; i<dictionary->n_bins;i++)
		{
			Node * n = dictionary->data[i];
			while(n!=NULL)
			{
				list_of_nodes[counter] = n;
				counter++;
				n = n->next;
			}
		//Erase old hashes
			dictionary->data[i] = NULL;
		}

		//Rehash elements
		int n_entries = dictionary->n_entries;
		dictionary->n_entries=0;
		for (int i=0; i<n_entries;i++)
			add_word(dictionary,list_of_nodes[i]->word, list_of_nodes[i]->definition);
	}

	unsigned int index = (*(dictionary->hash_algorithm))(word, dictionary->n_bins); 

	//If nothing in the destiny bucket, save there
	if(dictionary->data[index]==NULL)
	{
		Node* new_node = create_node(word,definition,index);
		dictionary->data[index] = new_node;
		dictionary->n_entries++;
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
		while(last_node->next!=NULL)
		{
			last_node = last_node->next;

			if(last_node && equal_words(last_node->word,word))
			{
				printf("Duplicate Key\n");
				return 1;
			}
		}
		
		Node * new_node= create_node(word, definition, index);
		last_node->next = new_node;
		dictionary->n_entries++;
	}

	return 0;

}

double calculate_bin_fraction(Hashmap* dictionary)
{
	int counter_bins_used =0;
	for(int i=0; i<dictionary->n_entries; i++)
	{
		if(dictionary->data[i])
		{
			printf("no vacio\n");
			counter_bins_used++;
		}
		else
			printf("vacio\n");
	}
	return (double)counter_bins_used/dictionary->n_bins;
}

int calculate_max_entries(Hashmap* dictionary)
{
	int max_entries =0;
	for(int i=0; i<dictionary->n_entries; i++)
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
	/*
Number of possible hash values (bins) currently allowable in the hash table.
• Occupancy (load factor) of the hash table. Occupancy = Number of entries being stored by hashtable Number of bins allowable in hashtable
• Fraction of bins that have one or more elements stored there = Number of bins being used in hashtable
Number of bins allowable in hashtable
• Maximum number of elements (words) currently stored in any single bin.

	*/
	int bins = dictionary->n_bins;
	double occupancy = (double)dictionary->n_entries/dictionary->n_bins;
	double used_bin_fraction = calculate_bin_fraction(dictionary);
	int max_entries = calculate_max_entries(dictionary);
	printf("Bins: %d, Occupancy: %f, Used bin fraction: %f, Max entries in a bin:%d\n",
			bins, occupancy, used_bin_fraction, max_entries);

	return 1;
}



int main(int argc, char **argv)
{
	int algorithm = atoi(argv[1]);

	//Instantiate inicial hashmap with 16 bins
	Hashmap * dictionary = NULL;
	dictionary = (Hashmap*)malloc(sizeof(Hashmap));
	dictionary->data = (Node**)malloc(16*sizeof(Node*));
	for(int i=0; i<16; i++)
		dictionary->data[i]=NULL;
	dictionary->n_bins = 16;
	dictionary->n_entries = 0;
	
	if(algorithm==1)
		dictionary->hash_algorithm = &naive_hash;
	else if (algorithm==2)
		dictionary->hash_algorithm = &bernstein_hash;
	else
		dictionary->hash_algorithm = &FNV_hash;


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
			printf("eee\n");
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
	}

	return 0;
}
