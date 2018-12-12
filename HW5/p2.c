#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>


#define TRUE 1
#define FALSE 0
#define MAXINT 10000000


typedef struct edgenode {
  int y;
  int weight;
  struct edgenode *next;
} Edgenode;

typedef struct {
  int nvertices;
  int nedges;
  int directed;
  Edgenode **edges;
} Graph;

//START HEAP STRUCTURE

// Structure to represent a min heap node 
typedef struct  
{ 
  int v; 
  int dist; 
} MinHeapNode; 

// Structure to represent a min heap 
typedef struct 
{ 
  int size;  // Number of heap nodes present currently 
  int capacity; // Capacity of min heap 
  int *pos;  // This is needed for decreaseKey()  // *****STILL DONT KNOW WHAT THIS IS
  MinHeapNode **array; 
} MinHeap; 

// A utility function to create a Min Heap 
MinHeap* createMinHeap(int capacity) 
{ 
  MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap)); 
  minHeap->size = 0; 
  minHeap->capacity = capacity; 

  minHeap->pos = (int *)malloc(capacity * sizeof(int));

  minHeap->array = (MinHeapNode**) malloc(capacity * sizeof(MinHeapNode*)); 
  
  return minHeap; 
} 

// A utility function to create a new Min Heap Node 
MinHeapNode* newMinHeapNode(int v, int dist) 
{ 
  MinHeapNode* minHeapNode =(MinHeapNode*) malloc(sizeof(MinHeapNode)); 
  minHeapNode->v = v; 
  minHeapNode->dist = dist; 
  return minHeapNode; 
}

// A utility function to swap two nodes of min heap. Needed for min heapify 
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) 
{ 
  MinHeapNode* t = *a; 
  *a = *b; 
  *b = t; 
}  

// Function to decreasy dist value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void decreaseKey(MinHeap* minHeap, int v, int dist) 
{ 
  // Get the index of v in heap array 
  int i = minHeap->pos[v];

  // Get the node and update its dist value 
  minHeap->array[i]->dist = dist; 

  // Travel up while the complete tree is not hepified. 
  // This is a O(Logn) loop 
  while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) 
  { 
    // Swap this node with its parent 
    minHeap->pos[minHeap->array[i]->v] = (i-1)/2; 
    minHeap->pos[minHeap->array[(i-1)/2]->v] = i; 
    swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]); 

    // move to parent index 
    i = (i - 1) / 2; 
  } 
} 


// A utility function to check if the given minHeap is ampty or not 
int isEmpty(MinHeap* minHeap) 
{ 
  return minHeap->size == 0; 
} 

// A standard function to heapify at given idx 
// This function also updates position of nodes when they are swapped. 
// Position is needed for decreaseKey() 
void minHeapify(MinHeap* minHeap, int idx) 
{ 
  int smallest, left, right; 
  smallest = idx; 
  left = 2 * idx + 1; 
  right = 2 * idx + 2; 

  if (left < minHeap->size && 
    minHeap->array[left]->dist < minHeap->array[smallest]->dist ) 
  smallest = left; 

  if (right < minHeap->size && 
    minHeap->array[right]->dist < minHeap->array[smallest]->dist ) 
  smallest = right; 

  if (smallest != idx) 
  { 
    // The nodes to be swapped in min heap 
    MinHeapNode *smallestNode = minHeap->array[smallest]; 
    MinHeapNode *idxNode = minHeap->array[idx]; 

    // Swap positions 
    minHeap->pos[smallestNode->v] = idx; 
    minHeap->pos[idxNode->v] = smallest; 

    // Swap nodes 
    swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); 

    minHeapify(minHeap, smallest); 
  } 
} 

// Standard function to extract minimum node from heap 
MinHeapNode* extractMin(MinHeap* minHeap) 
{ 
  if (isEmpty(minHeap)) 
    return NULL; 

  // Store the root node 
  MinHeapNode* root = minHeap->array[0]; 

  // Replace root node with last node 
  MinHeapNode* lastNode = minHeap->array[minHeap->size - 1]; 
  minHeap->array[0] = lastNode; 
  minHeap->array[minHeap->size - 1] = root; 

  // Update position of last node 
  minHeap->pos[root->v] = minHeap->size-1; 
  minHeap->pos[lastNode->v] = 0; 

  // Reduce heap size and heapify root 
  minHeap->size--; 
  minHeapify(minHeap, 0); 

  return root; 
} 

// A utility function to check if a given vertex 
// 'v' is in min heap or not 
int isInMinHeap(MinHeap *minHeap, int v) 
{ 
if (minHeap->pos[v] < minHeap->size) 
  return 1; 
return 0; 
} 

//END HEAP STRUCTURE


void free_heap(MinHeap* minHeap, int n)
{
  for (int i = 0; i < n; ++i)
    free(minHeap->array[i]);
 
  free(minHeap->pos);
  free(minHeap->array);
  free(minHeap);
}

//distance array of with ditances to each of the nodes from start in g
void dijkstra(Graph *g, int start, int *distance){
  
  //Set of all unvisited nodes in a heap
  MinHeap* minHeap = createMinHeap(g->nvertices);


  /* initialize search table and minheap elements */
  for (int i=0; i<g->nvertices; i++) {
    

    distance[i] = MAXINT;
    minHeap->array[i] = newMinHeapNode(i, distance[i]); 
    minHeap->pos[i] = i; 

  }

  // Initially size of min heap is equal to V 
  minHeap->size = g->nvertices;


  // Make dist value of src vertex as 0 so that it is extracted first 
  //minHeap->array[start] = newMinHeapNode(start, distance[start]); //ESTO LOS COMENTE YO XQ NO ME HACIAN SENTIDO
  //minHeap->pos[start] = start; 
  distance[start] = 0; 
  decreaseKey(minHeap, start, distance[start]); 


  // In the followin loop, min heap contains all nodes 
  // whose shortest distance is not yet finalized. 

  Edgenode *p; // Reference to next edge of the node we are looking at, u
  int w; // nodes adjacent to u
  int weight; // weight of that connection

  while (!isEmpty(minHeap))
  {
    // Extract the vertex with minimum distance value 
    MinHeapNode* minHeapNode = extractMin(minHeap); 
    int u = minHeapNode->v; // Store the extracted vertex number

    // Traverse through all adjacent vertices of u (the extracted 
    // vertex) and update their distance values 

    p = g->edges[u]; 
    
    //Update values in table
    while (p != NULL)
    {
      w = p->y;
      weight = p->weight;
      

      if(isInMinHeap(minHeap,w) && distance[w] > (distance[u]+weight)) 
      {
        //update distance
        distance[w] = distance[u]+weight;

        // update distance value in min heap also 
        decreaseKey(minHeap, w, distance[w]); 
      }

      p = p->next;
    }

  }

  free_heap(minHeap,g->nvertices);
}

void dijkstra_all_src(Graph* g, int** dist, int nthreads, int chuncksize)
{
  //calculate dijkstra from every node_i
  #pragma omp parallel for num_threads(nthreads) schedule(dynamic,chuncksize)
    for (int node_i = 0; node_i < g->nvertices; node_i++)
    {
      int my_id = omp_get_thread_num();
      //If the node exists
      if(g->edges[node_i])
      {
        //printf("Thread %d working on dijkstra form node %d\n", my_id, node_i);

        dijkstra(g, node_i, dist[node_i]);

        for (int i=0;i<g->nvertices;++i)
          if(g->edges[i])          
          {  //printf("d[%d]=%d\n", i,dist[node_i][i]);
          }
        //printf("\n");
      }
    }
}

int main(int argc, char **argv){
  Graph g;
  //Edgenode *e;
  FILE *file;
  char *filename;
  int u,v,w;
  int i;
  
  filename = argv[1];
  file = fopen(filename,"r");
  
  //Create graph

  int nv, ne;
  fscanf(file, "%d %d\n", &nv,&ne);  
  printf("nv:%d, ne:%d\n",nv,ne );



  //Distances from each node to all nodes
  int** dist = (int**) malloc(nv*sizeof(int*));
  for (i = 0; i < nv; ++i)
    dist[i] = (int*) malloc(nv*sizeof(int));


  /* initialize Graph fields */
  g.nvertices = nv;
  g.nedges = ne;
  g.directed = TRUE;

  g.edges = (Edgenode**)malloc((nv)*sizeof(Edgenode*)); //Sourcecode uses nv+1??
  for (i=0;i<nv;++i)
    g.edges[i] = NULL;


  while ( fscanf(file, "%d %d %d\n", &u,&v,&w) != EOF){
    //printf("%d %d %d\n", u,v,w);
    Edgenode *new_node = (Edgenode *) malloc(sizeof(Edgenode));          
    Edgenode *prev;
    if (g.edges[u] == NULL){             /* first insertion */
      g.edges[u]         = new_node;
      g.edges[u]->y       = v;
      g.edges[u]->weight = w;
      g.edges[u]->next    = NULL;
      prev = new_node;
    }
    else{                                /* subsequent insertions */
  
      prev->next          = new_node;
      new_node->y         = v;
      new_node->weight    = w;
      new_node->next      = NULL;
      prev                = new_node;
    }
  }
  fclose(file);

  printf("\n");



  double tf, ts;
  ts = omp_get_wtime();
  
  dijkstra_all_src(&g, dist, 4, 1);
  //dijkstra(&g, 0, dist[0]);

  tf = omp_get_wtime();

  printf("Work time parallel (openmp):%f sec\n", tf-ts);
}
