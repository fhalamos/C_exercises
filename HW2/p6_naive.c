/* Begin by importing your finished code from problem 4 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

#define WORD_LEN 64
#define DEF_LEN 1024
#define COMMAND_LEN 32


//IMPORTING CODE FROM PROBLEM SET 1, PROBLE 9: VECTOR.
typedef struct{
	double * data; /* Pointer to data */
	int size;      /* Size of the array from the user's perspective */
	int capacity;  /* The actual size of allocation "data" pointer points to */
} Vector;

/* Returns the value at given index of the vector */
double get( Vector * v, int index )
{
	return v->data[index];
}

/* Doubles the capacity of the vector */
/* If capacity is 0, increases capacity to 1 */
/* Used by the "insert" and "append" functions */
void resize( Vector * v )
{
	if(v->capacity==0)
	{
		v->capacity=1;
		v->data= (double *) malloc(sizeof(double));
	}
	else
	{
		v->capacity = v->capacity * 2;
		v->data= (double *) realloc(v->data, v->capacity*sizeof(double));
	}

}

/* Appends a value to the vector, automatically
 * resizing the vector if necessary by calling the
 * resize() function */
void append( Vector * v, double value )
{

	if(v->size==v->capacity)
	{
		resize(v);
	}
	v->data[v->size]=value;
	v->size++;
}

/* Initializes the vector to a specified size.
 * Capacity is set to be equal to the size of the vector.
 * All elements of the vector are initialized to 0. */
void init( Vector * v, int size )
{
	v->data= (double *) malloc(size*sizeof(double));
	v->size=size;
	v->capacity=size;

	int i;
	for(i=0;i<v->size;i++)
	{
		v->data[i]=0;
	}
}

/* Prints the vector in a clean format. If vector is empty,
 * just print "<  >" */
void print(Vector * v )
{
	int i;
	double value;
	printf("< ");
	
	for( i = 0; i < v->size; i++ )
	{
		value = get(v, i);
		printf("%f", value);
		if( i < v->size - 1 )
			printf(", ");
	}
	
	printf(" >\n");
/*	print_size_and_capacity(v);*/
}

double get_average(Vector * v )
{
	double sum=0;
	
	int i;
	for(i = 0; i < v->size; i++ )
		sum+=get(v, i);

	double average = sum/(double)v->size;	

	return average;
}




//NOW WE WILL CREATE THE SAME STRUCT, BUT TO BE A VECTOR OF char* RATHER THAN DOUBLE

//Struct will be Vector_words. Methods will end with _v_w
//Vector elements now are string instead of double
typedef struct{
	char ** data; /* Pointer to data */
	int size;      /* Size of the array from the user's perspective */
	int capacity;  /* The actual size of allocation "data" pointer points to */
} Vector_words;

/* Returns the value at given index of the vector */
char* get_v_w( Vector_words * v, int index )
{
	return v->data[index];
}


/* Doubles the capacity of the vector */
/* If capacity is 0, increases capacity to 1 */
/* Used by the "insert" and "append" functions */
void resize_v_w( Vector_words * v )
{
	if(v->capacity==0)
	{
		v->capacity=1;
		v->data= (char**) malloc(sizeof(char*));
	}
	else
	{
		v->capacity = v->capacity * 2;
		v->data= (char**) realloc(v->data, v->capacity*sizeof(char*));
	}

}

/* Appends a value to the vector, automatically
 * resizing the vector if necessary by calling the
 * resize() function */
void append_v_w( Vector_words * v, char* value )
{

	if(v->size==v->capacity)
	{
		resize_v_w(v);
	}
	v->data[v->size]=value;
	v->size++;
}


/* Initializes the vector to a specified size.
 * Capacity is set to be equal to the size of the vector.
 * All elements of the vector are initialized to 0. */
void init_v_w( Vector_words * v, int size )
{
	v->data= (char**) malloc(size*sizeof(char*));
	v->size=size;
	v->capacity=size;
}

/* Prints the vector in a clean format. If vector is empty,
 * just print "<  >" */
void print_v_w(Vector_words * v )
{
	int i;
	char* value;
	printf("< ");
	
	for( i = 0; i < v->size; i++ )
	{
		value = get_v_w(v, i);
		printf("%s", value);
		if( i < v->size - 1 )
			printf(", ");
	}
	
	printf(" >\n");
}

//END OF VECTOR OF WORDS CODE







typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	char word[WORD_LEN+1];      /* key   */
	char definition[DEF_LEN+1]; /* value */
} Tree_node;

int equal_words(char *str1, char* str2)
{
	if(strlen(str1)== strlen(str2) && strncmp(str1,str2,strlen(str2))==0)
		return 1;
	else
		return 0;
}

int add_word(Tree_node ** dictionary, char * word, char * definition)
{

	if(dictionary[0] ==NULL)
	{

		dictionary[0] = (Tree_node*)malloc(sizeof(Tree_node));

		memcpy(dictionary[0]->word, word, strlen(word)+1);
		memcpy(dictionary[0]->definition, definition, strlen(definition)+1);

		dictionary[0]->right=NULL;
		dictionary[0]->left=NULL;	

	}
	else
	{
		if(equal_words(dictionary[0]->word,word))
		{
			printf("Duplicate Key\n");
			return 1;
		}
		/*If key is higher that root, insert to the right*/
		else if(strncmp(dictionary[0]->word,word,strlen(word))<0)
		{
			add_word(&dictionary[0]->right,word,definition);
		}
		else
		{
			add_word(&dictionary[0]->left,word,definition);
		}
	}
	return 0;

}

int find_word(Tree_node * dictionary, char * word)
{
	if(dictionary==NULL)
		return 1;

	//If the word equals to the first node
	else if(equal_words(dictionary->word,word))
	{
		//printf("%s: %s\n", dictionary->word, dictionary->definition);
		return 0;
	}
	/*If key is higher that root, look to the right*/
	else if(strncmp(dictionary->word,word,strlen(word))<0)
	{
		return find_word(dictionary->right,word);
	}
	else
	{
		return find_word(dictionary->left,word);
	}

}


Tree_node* find_next(Tree_node *node)
{
	//When we call this function, we already know that the node has a right child
	Tree_node* next = node->right;

	while(!(next->left == NULL))
	{
		next = next->left;
	}	

	return next;
}

void free_node_recursively(Tree_node * node)
{
	if(node!=NULL)
		return;
	free_node_recursively(node->left);
	free_node_recursively(node->right);
	free(node);
	node=NULL;
}


void free_node(Tree_node * node)
{
	free(node);
}

Tree_node* get_parent(Tree_node * parent, Tree_node * node)
{
	if(strncmp(node->word,parent->word,strlen(node->word))<0)
	{
		if(equal_words(parent->left->word,node->word))
			return parent;
		else
			return(get_parent(parent->left,node));
	}
	else
	{
		if(equal_words(parent->right->word,node->word))
			return parent;
		else
			return(get_parent(parent->right,node));
	}
}

//lookAtParent == 1 -> study the parent too
int delete_word(Tree_node ** dictionary, Tree_node * parent, char * word, int lookAtParent)
{	
	if(dictionary == NULL || parent==NULL)
		return 1;


	//First lets check if the current parent is equal to the word
	if(*dictionary == parent && equal_words(parent->word,word) && lookAtParent)
	{

		//If its leaf, just tell the dictionary that there is no more root
		if(parent->left == NULL && parent->right== NULL)
		{
			dictionary[0]=NULL;
			free_node(parent);
			parent=NULL;
			return 0;
		}	
		
		//If it has only one right child
		else if(parent->left == NULL && parent->right!= NULL)
		{
			dictionary[0]=parent->right;
			free_node(parent);
			parent=NULL;
			return 0;
		}	
		//If it has only one left child
		else if(parent->left != NULL && parent->right== NULL)
		{
			dictionary[0]=parent->left;
			free_node(parent);
			parent=NULL;
			return 0;
		}
				
		//If child has two children
		else
		{
			Tree_node  *temp  =  find_next(parent);
			memcpy(parent->word, temp->word, strlen(temp->word));
			memcpy(parent->definition, temp->definition,strlen(temp->definition));
			return delete_word(dictionary, parent,temp->word, 0);
		}
	}

	//If the actual node does not equal the word, we look in its children
	else
	{
		Tree_node * left_child = parent->left;
		Tree_node * right_child = parent->right;
		Tree_node * child;
		//Decide which children must be looked
		//1 if we are looking to the left, 0 if to the right
		int is_left_child;
		if(strncmp(word,parent->word,strlen(word))<0)
		{
			child = left_child;
			is_left_child=1;
		}
		else
		{
			child = right_child;
			is_left_child=0;
		}

		if(child==NULL)
			return 1;

		//If the word is equal to the child
		if(equal_words(child->word,word))
		{
			//Depends if child is leaf, has one child, or has two children
			//If its leaf, just tell the parent that 
			//he has no left child any more
			if(child->left == NULL && child->right== NULL)
			{
				if(is_left_child==1)
					parent->left=NULL;
				else
					parent->right=NULL;
				
				free_node(child);
				return 0;
			}	

			//If it has only one right child
			else if(child->left == NULL && child->right!= NULL)
			{
				if(is_left_child==1)
					parent->left = child->right;
				else
					parent->right= child->right;
				
				free_node(child);
				return 0;
			}	

			//If it has only one left child
			else if(child->left != NULL && child->right== NULL)
			{
				if(is_left_child==1)
					parent->left = child->left;
				else
					parent->right= child->left;
				
				free_node(child);
				
				return 0;
			}
					
			//If child has two children
			else
			{
				Tree_node  *temp  =  find_next(child);

				memcpy(child->word, temp->word, strlen(temp->word));
				memcpy(child->definition, temp->definition,strlen(temp->definition));

				return delete_word(dictionary, child,temp->word,0);
			}
		}
		else
		{
			return delete_word(dictionary, child, word,0);
		}

	}
	

	
}


void print_dictionary(Tree_node * dictionary)
{
	if(dictionary!=NULL)
	{
		if(dictionary->left!=NULL)	
			print_dictionary(dictionary->left);
		
		printf("%s: %s\n", dictionary->word, dictionary->definition);
		
		if(dictionary->right!=NULL)	
			print_dictionary(dictionary->right);
	}
}



void clear_dictionary(Tree_node ** dictionary)
{
	free_node_recursively(dictionary[0]);
	dictionary[0]=NULL;
}


int import_file(char * fname, Tree_node ** dictionary )
{
    clock_t start_t, stop_t;
    double work_t;

    start_t = clock();

    FILE* file = fopen(fname, "r"); /* should check the result */
    char line[WORD_LEN + 3 + DEF_LEN];

    if(file==NULL)
    	return 1;

    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        
        if(line[strlen(line)-1]=='\n')
        	line[strlen(line)-1] = '\0';

    	char *word = strtok(line, " ");
    	char *definition= strtok(NULL, "");
  
    	add_word(dictionary, word, definition);
    }
    fclose(file);

    stop_t = clock();
    work_t = (double) (stop_t - start_t) / CLOCKS_PER_SEC;
    printf("Work time: %f sec\n", work_t );

    return 0;
}

void append_nodes_to_vector_recursively(Vector_words * v, Tree_node * dictionary)
{
	if(dictionary!=NULL)
	{
		if(dictionary->left!=NULL)	
			append_nodes_to_vector_recursively(v, dictionary->left);
		
		append_v_w(v, dictionary->word);
		
		if(dictionary->right!=NULL)	
			append_nodes_to_vector_recursively(v, dictionary->right);
	}	
}


int generate_random_number(int lower, int upper)
{
	//reference: https://www.geeksforgeeks.org/generating-random-number-range-c/
	int num = (rand() % (upper - lower + 1)) + lower; 
	return num;
}

void calculate_find_time(Tree_node * dictionary)
{
	srand(time(NULL));   // Initialization, should only be called once.

	//Save all elements of the dictionary in an array (vector)

	// Initialize Vector
	Vector_words vector_of_words;
	init_v_w(&vector_of_words, 0);
	
	// Append dictionary elements to vector
	append_nodes_to_vector_recursively(&vector_of_words, dictionary); 
	//print_v_w(&vector_of_words);

	Vector vector_of_times;
	init(&vector_of_times, 0);

	int i;
	//Generate 100.000 random numbers between (0, amount of words)
	for(i=0; i<100000;i++)
	{
		int start_t;
		int stop_t;
		double work_t;
		
		int random = generate_random_number(0,vector_of_words.size-1);
		//Get the word in vector indexed with the random number (random word)
		char* word_to_be_found = get_v_w(&vector_of_words,random);

		//printf("word_to_be_found:%s, index: %d\n", word_to_be_found, random);

		//Find the word in the tree 
		start_t = clock();
		find_word(dictionary, word_to_be_found);
		stop_t = clock();

		work_t = (double) (stop_t - start_t) / CLOCKS_PER_SEC;

		//printf("Work time: %f\n", work_t );

		//Save the time that it took to find that number
		append(&vector_of_times, work_t);
		//print(&vector_of_times);
	}

	//Take average 
	double av = get_average(&vector_of_times);
	printf("average time: %f us\n", av*1000000);
}


int main(void)
{

	Tree_node * dictionary = NULL;

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
			error = add_word(&dictionary, word, definition);

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
			error = import_file(fname, &dictionary );
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
			error = delete_word(&dictionary, dictionary, word,1);
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
			clear_dictionary(&dictionary);
		/* quit program */
		if( strncmp(command, "quit", COMMAND_LEN) == 0 )
			break;

		if( strncmp(command, "calculate_find_time", COMMAND_LEN) == 0 )
			calculate_find_time(dictionary);
	}

	return 0;
}
