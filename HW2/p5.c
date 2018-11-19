#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define WORD_LEN 64
#define DEF_LEN 1024
#define COMMAND_LEN 32


const int BLACK = 1;
const int RED = 0;

typedef struct tree_node{

	struct tree_node * left;
	struct tree_node * right;
	struct tree_node * parent;
	char word[WORD_LEN+1];      /* key   */
	char definition[DEF_LEN+1]; /* value */
	int color; //1 if black, 0 if red

} Tree_node;

// 1 if okay 0 otherwise
int tree_okay(Tree_node *n) {
	if (n == NULL)
		return 1;
	if (n->color == 1)
	{	
		if(n->left && n->left->parent != n)
			return 0;
		if(n->right && n->right->parent != n)
			return 0;
		return tree_okay(n->left) && tree_okay(n->right);
	}
	else
	{
		if (n->left && n->left->color == 0)
			return 0;
		if (n->right && n->right->color == 0)
			return 0;
		if(n->left && n->left->parent != n)
			return 0;
		if(n->right && n->right->parent != n)
			return 0;
		return tree_okay(n->left) && tree_okay(n->right);
	}
}


Tree_node* grandparent (Tree_node  *n)
{
	return n->parent->parent;

}

Tree_node* uncle (Tree_node  *n)
{
	Tree_node* g = grandparent(n);
	if(g->left == n->parent)
		return g->right;
	else
		return g->left;
}



void rotate_left(Tree_node  *n)
{
	Tree_node *tmp_node;
	
	char* tmp_word = (char *) malloc((strlen(n->word)+1)*sizeof(char));
	char* tmp_def = (char *) malloc((strlen(n->definition)+1)*sizeof(char));
	int tmp_color;

	tmp_node = n->left;
	
	memcpy(tmp_word, n->word, strlen(n->word)+1);
	memcpy(tmp_def, n->definition, strlen(n->definition)+1);
	//Added this line, comparing to slides
	tmp_color = n->color; 

	n->left = n->right;

	memcpy(n->word,n->right->word,strlen(n->right->word)+1);
	memcpy(n->definition,n->right->definition,strlen(n->right->definition)+1);
	//Added this line, comparing to slides
	n->color = n->right->color;	

	n->right = n->left->right;
	//Added this line, comparing to slides
	if(n->right!=NULL)
		n->right->parent = n;

	n->left->right = n->left->left;

	n->left->left = tmp_node;
	//Added this line, comparing to slides
	if(tmp_node!=NULL)
		tmp_node->parent = n->left;
	
	memcpy(n->left->word, tmp_word, strlen(tmp_word)+1);
	memcpy(n->left->definition, tmp_def, strlen(tmp_def)+1);
	n->left->color=tmp_color;

	free(tmp_word);
	free(tmp_def);
}  

void rotate_right(Tree_node *n)
{
	Tree_node *tmp_node;
	
	char* tmp_word = (char *) malloc((strlen(n->word)+1)*sizeof(char));
	char* tmp_def = (char *) malloc((strlen(n->definition)+1)*sizeof(char));
	int tmp_color;

	tmp_node = n->right;
	
	memcpy(tmp_word, n->word, strlen(n->word)+1);
	memcpy(tmp_def, n->definition, strlen(n->definition)+1);
	tmp_color = n->color;

	n->right = n->left;

	memcpy(n->word, n->left->word, strlen(n->left->word)+1);
	memcpy(n->definition, n->left->definition, strlen(n->left->definition)+1);
	n->color = n->left->color;

	n->left = n->right->left;
	if(n->right->left != NULL)
		n->left->parent = n;
	
	n->right->left = n->right->right;
	
	n->right->right = tmp_node;
	if(tmp_node!=NULL)
		tmp_node->parent = n->right;

	memcpy(n->right->word, tmp_word, strlen(tmp_word)+1);
	memcpy(n->right->definition, tmp_def, strlen(tmp_def)+1);
	n->right->color=tmp_color;

	free(tmp_word);
	free(tmp_def);
} 

int equal_words(char *str1, char* str2)
{
	if(strlen(str1)== strlen(str2) && strncmp(str1,str2,strlen(str2))==0)
		return 1;
	else
		return 0;
}

void insert_case5(Tree_node  *n)
{
	
	Tree_node *g = grandparent(n);

	n->parent->color = BLACK;
	g->color = RED;
	
	
	if (n == n->parent->left)
		rotate_right(g);

	else // n == n->parent->right
		rotate_left(g);
		
} 

void insert_case4(Tree_node *n)
{
	
	Tree_node *g = grandparent(n);

	if((n == n->parent->right) && (n->parent == g->left))
	{

		rotate_left(n->parent);

		//This line was in the lecture notes but i believe shall not be used
		//cause n is already the left side child of the node which we used
		//as pivot for the rotation
		
		//n = n->left;

	}
	else if((n == n->parent->left) && (n->parent == g->right))
		rotate_right(n->parent);
		//n = n->right;

	insert_case5(n);
}  

void insert_case3(Tree_node *n)
{
	
	Tree_node * u = uncle(n);
	Tree_node * g;

	if((u!=NULL)&& (u->color==RED))
	{
		n->parent->color = BLACK;
		u->color = BLACK;
		g = grandparent(n);
		g->color = RED;
		insert_case1(g);
	}
	else
		insert_case4(n); 
	
}

void insert_case2(Tree_node *n) 
{
	if(n->parent->color==BLACK)
		return;
	else
		insert_case3(n);
}

void insert_case1(Tree_node *n) 
{

	if(n->parent == NULL)
		n->color = BLACK;   
	else
		insert_case2(n);
} 


int add_node(Tree_node ** dictionary, Tree_node * new_node)
{

	Tree_node* parent = dictionary[0];

	if(parent ==NULL)
	{
		new_node->parent=NULL;
		insert_case1(new_node);
		dictionary[0] = new_node;
	}

	else
	{

		if(equal_words(parent->word,new_node->word))
		{
			printf("Duplicate Key\n");
			return 1;
		}

		/*If key is higher that root, insert to the right*/
		else if(strncmp(parent->word,new_node->word,strlen(new_node->word))<0)
		{
			if(parent->right==NULL)
			{	
				parent->right=new_node;
				new_node->parent=parent;

				insert_case1(new_node);

			}
			else
			{
				add_node(&parent->right,new_node);
			}
		}
		else
		{
			if(parent->left==NULL)
			{	
				parent->left=new_node;
				new_node->parent=parent;
				insert_case1(new_node);
			}
			else
			{
				add_node(&parent->left,new_node);
			}
		}
	}
	return 0;
}

int add_word (Tree_node ** dictionary, char * word, char * definition)
{

	Tree_node* new_node = (Tree_node*)malloc(sizeof(Tree_node));
	
	memcpy(new_node->word, word, strlen(word)+1);
	memcpy(new_node->definition, definition, strlen(definition)+1);
	new_node->color=RED;

	new_node->right=NULL;
	new_node->left=NULL;	

	return add_node(dictionary, new_node);

}

/* This should be identical to your find_word() function from Problem 4 */
int find_word(Tree_node * dictionary, char * word)
{
	if(dictionary==NULL)
		return 1;

	//If the word equals to the first node
	else if(equal_words(dictionary->word,word))
	{
		printf("%s: %s\n", dictionary->word, dictionary->definition);
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

/* This should be identical to your print_dictionary() function from Problem 4 */
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

/* This should be identical to your clear_dictionary() function from Problem 4 */
void clear_dictionary(Tree_node ** dictionary)
{
	free_node_recursively(dictionary[0]);
	dictionary[0]=NULL;
}

/* This should be identical to your import_file() function from Problem 4 */
int import_file(char * fname, Tree_node ** dictionary )
{
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
    return 0;
}

int main(void)
{
	Tree_node * dictionary = NULL;

	while(1)
	{
		int error;
		char command[COMMAND_LEN];

		/* Get the Users Command */
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
			{
				printf("The word \"%s\" has been added successfully.\n", word);
				//pretty_print_dictionary(dictionary,0);
			}
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

	}

	return 0;
}
