#include<stdio.h>
#include<stdlib.h>
//#define NV 6301
//#define NE 20777
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

void dijkstra(Graph *g, int start, int *distance){
  int i;
  Edgenode *p;
  int processed[g->nvertices+1];
  int v;
  int w;
  int weight;
  int dist;

  /* initialize search table */
  for (i=0; i<=g->nvertices; i++) {
    processed[i] = FALSE;
    distance[i] = MAXINT;
  }
  distance[start] = 0;
  v = start;
  
  /* add v to processed list and update distance of neighbors */
  while (processed[v] == FALSE) {
    processed[v] = TRUE;
    p = g->edges[v];    /* adjacency list for current node */
    
    //Update values in table
    while (p != NULL) {
      w = p->y;
      weight = p->weight;
      if (distance[w] > (distance[v]+weight)) 
        distance[w] = distance[v]+weight;
      p = p->next;
    }


    /* find next closest vertex */
    /* note: this loop is O(n), can do better */

    //PENDIENTE!!! Priority queue for the next smallest node search, have to clarify that in Q1 writeup
    for (i=0,v=1,dist=MAXINT; i<=g->nvertices; i++)
      if ((processed[i] == FALSE) && (dist > distance[i])) {
      	dist = distance[i];
      	v = i; 
      }
  }
}

void dijkstra_all_src(Graph* g, int** dist)
{
  //calculate dijkstra from every node_i
  for (int node_i = 0; node_i < g->nvertices; node_i++)
  {
    //If the node exists
    if(g->edges[node_i])
    {
      printf("Dikjsta from node %d\n", node_i);

      dijkstra(g, node_i, dist[node_i]);

      for (int i=0;i<g->nvertices;++i)
        if(g->edges[i])
          printf("d[%d]=%d\n", i,dist[node_i][i]);
      printf("\n");
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
  g.edges = (Edgenode**)malloc(nv*sizeof(Edgenode*)); //Sourcecode uses nv+1??
  for (i=0;i<nv;++i)
    g.edges[i] = NULL;


  while ( fscanf(file, "%d %d %d\n", &u,&v,&w) != EOF){
    printf("%d %d %d\n", u,v,w);
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
  dijkstra_all_src(&g, dist);

}
