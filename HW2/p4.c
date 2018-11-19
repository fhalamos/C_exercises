#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define WORD_LEN 64
#define DEF_LEN 1024
#define COMMAND_LEN 32


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
	}

	return 0;
}
