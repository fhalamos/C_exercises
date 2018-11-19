#include <stdio.h>
#include <stdlib.h>

/*Array
3		1		2		7		4		0		2
a[0]	a[1]	a[2]	a[3]	a[4]	a[5]	a[6]	*/

/*Considering that the array starts in index 0*/
int parent (int i)
{
	return (int)((i-1)/2);
}

int leftChild (int i)
{
	return 2*i+1;
}

int rightChild (int i)
{
	return 2*i+2;
}


/*Swaps values in f[a] for f[b]*/
void swap(int *f, int a, int b)
{
	int temp = f[a];
	f[a] = f[b];
	f[b] = temp;
}


/*This function repairs the subheap whose root element is at index 'startItem'
It assumes that the sub-heaps rooted with the children of the startItem node
are valid. Hence, if the relationships between startItem and its direct
children are OK, the whole heap can be assumed to be OK.

If the relationship is not OK, we will move the node at index 'startItem' to a place
such that all nodes below him are in heap order (lower value)

f is the array, n is the total number of items in the array

*/
void siftDown(int *f, int startItem, int n)
{

	/*itemToSwap is the index of the item with which we will have to swap startItem
	It is initially equal to startItem,
	so that if they end up being equal there is nothing to do*/
	int itemToSwap = startItem;

	/*Check left child*/
	int left_child_index= leftChild(startItem); 
	
	if(left_child_index<n)
	{
		/*If the left child is bigger than the father, we need to save it 
		so as to be swaped later*/

		if(f[left_child_index]>f[startItem])
		{
			itemToSwap = left_child_index;
		}
	}

	/*Check right child*/
	int right_child_index;
	right_child_index= rightChild(startItem); 

	if(right_child_index<n)
	{
		/*If the right child is bigger than the actual itemToSwap (either the father
		or the left children), we need to save it 
		so as to be swaped later*/

		if(f[right_child_index]>f[itemToSwap])
		{
			itemToSwap = right_child_index;
		}
	}

	/*If any of the two children were bigger than the father, we swap the biggest one with 
	the father, and	then we check that the new subheap with the old father 
	as root is valid*/
	if(itemToSwap!=startItem)
	{
		swap(f,startItem,itemToSwap);		
		siftDown(f,itemToSwap,n);
	}

}


/*f is the array to be transformed into a heap.
n is the number of elements	*/
void heapify(int *f, int n)
{
	/*We start looking at the deepest parent, and then go up till the first element.
	For each parent, we check that from there down the heap is valid.*/
	int currentParentIndex = parent(n-1);
	while(currentParentIndex>=0)
	{
		/*We rearrange the heap, from currentParentIndex index downwards -
		in other worths, we rearrange
		the subheap that has currentParentIndex as root node*/
        siftDown(f, currentParentIndex, n);

        currentParentIndex = currentParentIndex - 1;
	}
}


/*Takes the integers as input from argv and copies them to a separate 
dynamically allocated array after converting with atoi()*/
int * allocate_array( int argc, char * argv[])
{
	/*Create the necessary space in memory*/
	/*There are argc-1 element*/
	int * array = malloc((argc-1) * sizeof(int));
	
    int i;
    /*Elements are in argv starting from index 1*/
	for(i = 0; i < argc-1; i++ )
	{
      /*Parse each argument to int and save in corresponding memory slot*/
      array[i]= atoi(argv[i+1]);
   	}

   	return array;

}


int test_heap( int * f, int n)
{
	int i;
	int child_A, child_B;

	for( i = 0; i < n; i++ )
	{
		child_A = ( 2 * i ) + 1;
		child_B = ( 2 * i ) + 2;

		if( child_A < n )
			if( f[i] < f[child_A] )
				return -1;
		
		if( child_B < n )
			if( f[i] < f[child_B] )
				return -1;
	}

	return 1;
}






int main(int argc, char * argv[])
{
	int result;
	int i;
	int n = argc - 1;
	int * arr = allocate_array( argc, argv );

	heapify( arr, n );
	result = test_heap( arr, n );

	for( i = 0; i < n; i++ )
		printf("%d ", arr[i]);
	printf("\n");

	if( result == 1 )
		printf("Heap test success!\n");
	else
		printf("Heap test failed.\n");

	free(arr);

	return 0;
}