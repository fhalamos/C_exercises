#include<stdio.h>
#include<stdlib.h>

/*Nodes of the mega_array contain:
-A float number: which comes from the number of the originals arrays
-An id which indicated from which original array that number comes
-A position in its original array
-An array of ids to other nodes: to have a connection to the nodes to its left
that belong to other original arrays. This id is respect to the mega_array.
*/
typedef struct node{
	float number;
	int id_original_array;
	int position_original_array; 
	int *id_closest_nodes_of_other_arrays;

} Node;


typedef struct{
	float ** arrays;
	int m;   /* Number of arrays */
	int * n; /* Length of each array */

	//We will have an array that contains all Nodes
	//For each number of each array there will be a node
	Node ** mega_array;
	int total_amount_elements;

} Grid;


int improved_recursive_binary_search(Node ** array, int lower, int upper, float p)
{
	
	if(lower==upper || lower+1 == upper)
		return lower;

	else if(p == array[lower + (upper-lower)/2]->number)
		return lower + (upper-lower)/2;
	
	else if(p < array[lower + (upper-lower)/2]->number)
		return improved_recursive_binary_search(array, lower, lower + (upper-lower)/2, p);
	else
		return improved_recursive_binary_search(array, lower + (upper-lower)/2, upper, p);
}

/*
We will do a binary search, same as in p7, but this time we will search in the
mega_array (array that contains all the numbers of all arrays). 
Hence, we will only do ONE binary search in an array of n*m elements - before we did m binary searches in arrays of length n.

We will then find the lower bound in time O(log (m*n)).

Once we find the lower bound, we must consider that this bound may be of any of the nodes among the total nodes.
Hence, we will ask that node who is the closest node to its left that belongs to the same array that we are looking for.
We do this for every array so as to output the result. This operatios is O(m), and is mandatory just because we
need to save the values in each position of results
*/
void improved_grid_search( float p, Grid grid, int * results )
{
	
	//This is the lower bound, but from the total array.

	int lower_bound = improved_recursive_binary_search(	grid.mega_array,
														0,
														grid.total_amount_elements-1,
														p);

	//For each of the original arrays
	for(int i=0; i<grid.m; i++)
	{
		//We need to ask the lower bound who his closest element of array i is
		int id_closest_node = grid.mega_array[lower_bound]->id_closest_nodes_of_other_arrays[i];

		//Once we know that node to the left of the lower bound, we ask him his position in his original array.
		results[i] = grid.mega_array[id_closest_node]->position_original_array;
	}
}



void order_array(Node ** mega_array, int size)
{

	for(int i =0; i<size; i++)
	{
		Node* tmp = mega_array[i];
		Node* min_node = mega_array[i];

		int index_min_node=i;

		for(int j=i+1; j<size; j++)
		{
			Node* a = mega_array[j];
	
			if(a->number < min_node->number)
			{
				min_node = a;
				index_min_node = j;
			}
		}

		if(i!=index_min_node)
		{
			mega_array[i]=min_node;
			mega_array[index_min_node]=tmp;
		}
	}


}

void print_mega_array(Node ** mega_array, int size)
{
	printf("Mega array:\n");
	for(int i=0; i<size; i++)
		printf("Number: %f, Position: %d, Array: %d, Position there: %d\n", mega_array[i]->number, i, mega_array[i]->id_original_array, mega_array[i]->position_original_array);
	printf("\n");
}

void print_mega_array_with_pointers(Node ** mega_array, int n_original_arrays, int size)
{
	printf("Mega array:\n");
	for(int i=0; i<size; i++)
	{
		printf("Number: %f, Position: %d, Array: %d, Position there: %d\n", mega_array[i]->number, i, mega_array[i]->id_original_array, mega_array[i]->position_original_array);
		
		for(int j=0; j<n_original_arrays; j++)
		{
			printf("Index of closest element of array %d: %d\n", j, mega_array[i]->id_closest_nodes_of_other_arrays[j]);
		}	
		printf("\n");

	}
	printf("\n");
	printf("\n");
}

void init_improved_grid_search( Grid * grid )
{
	//Calculate the total amount of numbers among all arrays
	int quantity_of_numbers=0;
	for(int i=0; i<grid->m; i++)
		quantity_of_numbers+=grid->n[i];

	//Allocate space for all the nodes (1 node per number)
	grid->mega_array = (Node **) malloc (quantity_of_numbers*sizeof(Node*));
	grid->total_amount_elements = quantity_of_numbers;

	//Counter of total nodes we have already created
	int counter_nodes=0;

	//For each number, we create a node
	//For each array
	for(int i=0; i<grid->m; i++)
	{
		//For each number in this array
		for(int j=0; j<grid->n[i]; j++)
		{
			//Allocate node
			Node* node = (Node *) malloc(sizeof(Node));

			//Assign number to node
			node->number = grid->arrays[i][j];
			
			//Assign the id of the original array
			node->id_original_array=i;

			//Assing its position in its original array
			node->position_original_array=j;

			grid->mega_array[counter_nodes]=node;
			counter_nodes++;
		}
	}
	
	//Order the mega array
	order_array(grid->mega_array, counter_nodes);


	//Setting connections to other nodes

	//Array of ids of the closest nodes of each of the original arrays from the node we are visiting
	int* id_of_closest_node_from_each_arrays = (int*) malloc(grid->m*sizeof(int));
	//Set all to -1 initially.
	for(int i=0; i<grid->m; i++)
		id_of_closest_node_from_each_arrays[i] = -1;

	
	for(int i=0; i<counter_nodes;i++)
	{
		Node* node = grid->mega_array[i];
		node->id_closest_nodes_of_other_arrays = (int *) malloc(grid->m * sizeof (int));

		//Save the position of this node as the closest node of his array
		id_of_closest_node_from_each_arrays[node->id_original_array]=i;

		//Tell this node which nodes to his left are the closest, of each original array
		for(int k=0; k<grid->m; k++)
		{
			node->id_closest_nodes_of_other_arrays[k] = id_of_closest_node_from_each_arrays[k];
		}
		
	}

}


/* Builds and returns a small test grid */
Grid * build_test_grid(void)
{
	Grid * grid = (Grid *) malloc(sizeof(Grid));
	grid->m = 5;
	grid->n = (int *) malloc( grid->m * sizeof(int));

	float ** a = (float **) malloc( grid->m * sizeof(float *)); 
	grid->n[0] = 6; grid->n[1] = 7; grid->n[2] = 4; grid->n[3] = 6; grid->n[4] = 8;
	a[0] = (float *) malloc(6 * sizeof(float));
	a[1] = (float *) malloc(7 * sizeof(float));
	a[2] = (float *) malloc(4 * sizeof(float));
	a[3] = (float *) malloc(6 * sizeof(float));
	a[4] = (float *) malloc(8 * sizeof(float));

	a[0][0] = 0.00; a[0][1] = 0.13; a[0][2] = 0.32; a[0][3] = 0.38; a[0][4] = 0.57; a[0][5] = 1.00;
	a[1][0] = 0.00; a[1][1] = 0.23; a[1][2] = 0.34; a[1][3] = 0.68; a[1][4] = 0.69; a[1][5] = 0.85; a[1][6] = 1.00;
	a[2][0] = 0.00; a[2][1] = 0.03; a[2][2] = 0.67; a[2][3] = 1.00;
	a[3][0] = 0.00; a[3][1] = 0.15; a[3][2] = 0.16; a[3][3] = 0.45; a[3][4] = 0.93; a[3][5] = 1.00;
	a[4][0] = 0.00; a[4][1] = 0.35; a[4][2] = 0.78; a[4][3] = 0.90; a[4][4] = 0.95; a[4][5] = 0.96; a[4][6] = 0.99; a[4][7] = 1.00;

	grid->arrays = a;

	return grid;
}

int main(void)
{
	Grid * grid = build_test_grid();
	init_improved_grid_search( grid );
	int * results = (int *) malloc( grid->m * sizeof(int));

	/* First test */
	float p = 0.23;
	improved_grid_search(p, *grid, results);
	printf("Test p = %.2f\n", p);
	printf("Expected: 1 1 1 2 0\n");
	printf("Results : ");
	for( int i = 0; i < grid->m; i++ )
		printf("%d ", results[i]);
	printf("\n");

	/* Second test */
	p = 0.94;
	improved_grid_search(p, *grid, results);
	printf("Test p = %.2f\n", p);
	printf("Expected: 4 5 2 4 3\n");
	printf("Results : ");
	for( int i = 0; i < grid->m; i++ )
		printf("%d ", results[i]);
	printf("\n");

	/* Third test */
	p = 0.54;
	improved_grid_search(p, *grid, results);
	printf("Test p = %.2f\n", p);
	printf("Expected: 3 2 1 3 1\n");
	printf("Results : ");
	for( int i = 0; i < grid->m; i++ )
		printf("%d ", results[i]);
	printf("\n");

	return 0;
}
