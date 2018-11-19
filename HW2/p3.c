#include<stdio.h>
#include<stdlib.h>

typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	int data;
} Tree_node;

int has_no_children(Tree_node * tree)
{
	if(tree->left == NULL && tree->right==NULL)
		return 1;
	else
		return 0;
}

int has_only_right_children(Tree_node *tree)
{
	if(tree->right != NULL && tree->left==NULL)
		return 1;
	else
		return 0;
}

int has_only_left_children(Tree_node *tree)
{
	if(tree->left != NULL && tree->right==NULL)
		return 1;
	else
		return 0;
}

//Function assumes the trees are valid.
int is_same(Tree_node * tree1, Tree_node * tree2)
{
	//If values of the root are !=, return false
	if(tree1->data !=tree2->data)
		return 0;
	else
	{
		if(has_no_children(tree1) && has_no_children(tree2))
			return 1;
		
		else if((has_only_right_children(tree1) && has_only_left_children(tree2)) ||
				(has_only_left_children(tree1) && has_only_right_children(tree2)))
			return 0;

		else if(has_only_right_children(tree1) && has_only_right_children(tree2))
			return is_same(tree1->right, tree2->right);
		
		else if(has_only_left_children(tree1) && has_only_left_children(tree2))
			return is_same(tree1->left, tree2->left);

		else
			return is_same(tree1->left, tree2->left) && is_same(tree1->right, tree2->right);
	}
	

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
	Tree_node * tree1 = build_good_tree();
	Tree_node * tree2 = build_bad_tree();

	int identical = is_same(tree1, tree1);
	if( identical )
		printf("Trees are the same!\n");
	else
		printf("Trees are different!\n");

	identical = is_same(tree2, tree2);
	if( identical )
		printf("Trees are the same!\n");
	else
		printf("Trees are different!\n");

	identical = is_same(tree1, tree2);
	if( identical )
		printf("Trees are the same!\n");
	else
		printf("Trees are different!\n");

	identical = is_same(tree2, tree1);
	if( identical )
		printf("Trees are the same!\n");
	else
		printf("Trees are different!\n");


	return 0;
}
