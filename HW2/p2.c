#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	int data;
} Tree_node;

void print_tree_node(Tree_node * node)
{
	printf("Data nodo:%d\n", node->data);
}

/* Adapted Vector type, with data being an array of node pointers */
typedef struct{
	Tree_node ** data; /* Pointer to data */
	int size;      /* Size of the array from the user's perspective */
	int capacity;  /* The actual size of allocation "data" pointer points to */
} Vector;


/* Returns the node at given index of the vector */
Tree_node* get( Vector * v, int index )
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
		v->data= (Tree_node **) malloc(sizeof(Tree_node*));
		
	}
	else
	{
		v->capacity = v->capacity * 2;
		v->data= (Tree_node **) realloc(v->data, v->capacity*sizeof(Tree_node*));
	}

}

/* Appends a node to the vector, automatically
 * resizing the vector if necessary by calling the
 * resize() function */
void append( Vector * v, Tree_node *node )
{
	if(v->size==v->capacity)
	{
		resize(v);
	}
	v->data[v->size]=node;
	v->size++;
}

/* Deletes the given index, and moves all nodes after
 * the index to the left by one element.
 * This reduces the size of the vector by 1, but the
 * capacity should remain the same */
void delete( Vector * v, int index )
{
	int i;
	for(i=index; i<v->size-1; i++)
	{
		v->data[i]=v->data[i+1];
	}
	v->size--;
}

/* Initializes the vector to a specified size.
 * Capacity is set to be equal to the size of the vector. */
void init( Vector * v, int size )
{
	v->data= (Tree_node **) malloc(size*sizeof(Tree_node*));
	v->size=size;
	v->capacity=size;
}

/* Prints the vector in a clean format. If vector is empty,
 * just print "<  >" */
void print_vector(Vector * v )
{
	int i;
	int value;
	Tree_node* node;
	printf("< ");
	
	for( i = 0; i < v->size; i++ )
	{
		node = get(v, i);
		value = node->data;
		printf("%d", value);
		if( i < v->size - 1 )
			printf(", ");
	}
	
	printf(" >\n");
}


/* Stack structure built on top of our Vector type */
typedef struct{
	Vector * v;
} Stack;

/* Initializes stack */
Stack * init_stack(void)
{
	Stack* s = (Stack *) malloc(sizeof(Stack));
	s->v = (Vector*) malloc(sizeof(Vector));
	init(s->v,0);
	return s;
}

void print_stack(Stack* s)
{
	print_vector(s->v);
}

/* Pushes node pointer onto stack */
void push(Stack * stack, Tree_node * node )
{
	//Insert the element in last position of the vector
	append(stack->v, node);
}

/* Pops node pointer off of stack */
Tree_node * pop( Stack * stack )
{
	//Get the node in the last position of the vector, and then delete the element
	
	Tree_node* node = get(stack->v, stack->v->size-1);
	delete(stack->v,stack->v->size-1);
	return node;
}


/* Determines if the given tree is a valid BST or not */
int is_tree_iterative(Tree_node * tree)
{

/*
1) 	Create an empty stack S.
2) 	Initialize current node as root
3) 	Push the current node to S and set current = current->left (check property)
	until current is NULL: Traverse all left children in depth
4) 	If current is NULL and stack is not empty then 
     a) Pop the top item from stack.
     b) Print the popped item, set current = popped_item->right (check property)
     c) Go to step 3: traverse all left children of the new node
     5) If current is NULL and stack is empty then we are done.
*/	



	Stack* stack;
	stack = init_stack();


	Tree_node* current = tree;

	int previous_value = INT_MIN;

	while(stack->v->size!=0 || current)
	{
		//Traverse all left children in depth
		if(current !=NULL)
		{
			push(stack,current);
			Tree_node* left_child = current->left;
			current = left_child;
		}

		//When there are no more left children, get the last element in the
		//stack and look once to the right. Then, start again looking to the left
		else
		{
			Tree_node* node = pop(stack);
			
			//If the new value to extract is smaller than the previous found, 
			//the BST is not valid. This exercise should return an ordered list
			if(node->data < previous_value)
				return 0;

			previous_value = node->data;
			current = node->right;
		}

	}
	
	return 1;
}




Tree_node * build_good_tree(void)
{
	/* Allocate head */
	int n_nodes = 11;
	int i;
	Tree_node * nodes = (Tree_node *) malloc(n_nodes * sizeof(Tree_node));
	for( i = 0; i < n_nodes; i++ )
	{
		nodes[i].data  = i;
		nodes[i].left  = NULL;
		nodes[i].right = NULL;
	}

	/* Top Level */
	nodes[5].left =  &nodes[3];
	nodes[5].right = &nodes[7];
	/* Second Level */
	nodes[3].left =  &nodes[1];
	nodes[3].right = &nodes[4];
	nodes[7].left =  &nodes[6];
	nodes[7].right = &nodes[9];
	/* Third Level */
	nodes[1].left =  &nodes[0];
	nodes[1].right = &nodes[2];
	nodes[9].left =  &nodes[8];
	nodes[9].right = &nodes[10];
	/* Everything else is a leaf */

	return &nodes[5];
}

Tree_node * build_bad_tree(void)
{
	/* Allocate head */
	int n_nodes = 11;
	int i;
	Tree_node * nodes = (Tree_node *) malloc(n_nodes * sizeof(Tree_node));
	for( i = 0; i < n_nodes; i++ )
	{
		nodes[i].data  = i;
		nodes[i].left  = NULL;
		nodes[i].right = NULL;
	}

	/* Top Level */
	nodes[5].left =  &nodes[3];
	nodes[5].right = &nodes[7];
	/* Second Level */
	nodes[3].left =  &nodes[1];
	nodes[3].right = &nodes[6];
	nodes[7].left =  &nodes[6];
	nodes[7].right = &nodes[9];
	/* Third Level */
	nodes[1].left =  &nodes[0];
	nodes[1].right = &nodes[2];
	nodes[9].left =  &nodes[3];
	nodes[9].right = &nodes[10];

	return &nodes[5];
}

int main(void)
{
	Tree_node * good_test_tree = build_good_tree();
	Tree_node * bad_test_tree;

	int isBST = is_tree_iterative(good_test_tree);


	if( isBST )
		printf("Tree is valid!\n");
	else
		printf("Tree is invalid!\n");

	bad_test_tree = build_bad_tree();

	isBST = is_tree_iterative(bad_test_tree);

	if( isBST )
		printf("Tree is valid!\n");
	else
		printf("Tree is invalid!\n");


	return 0;
}
