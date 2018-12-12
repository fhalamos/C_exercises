/*
Write a function graph period that efficiently calculates the period of a 
directed graph G. Then
write a driver that takes the Markov matrix in the hw directory as
input and calculates its period
(as a reminder aperiodic is equivalent to a period of 1). Name this source
file p4.c
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define MAX_LINES 1000
#define MAX_CHAR_PER_LINE 1000

typedef struct edgenode {
  int y;
  double weight;
  struct edgenode *next;
} Edgenode;

typedef struct {
  int nvertices;
  int nedges;
  int directed;
  Edgenode **edges;
} Graph;


void add_edge_to_graph(Graph * g, int u, int v, int w)
{
  Edgenode *new_node = (Edgenode *) malloc(sizeof(Edgenode)); 
  new_node->y         = v;
  new_node->weight    = w;
  new_node->next      = NULL;

  Edgenode *last_node = g->edges[u];
  if (g->edges[u] == NULL){            
    g->edges[u] = new_node;
  }
  else{
    while(last_node->next != NULL)
      last_node = last_node->next;

    last_node->next = new_node;
  }
}

void dfs(Graph *g, int v, int l, int* discovered, int* level){
  Edgenode *p;
  int y;
  double w;
  
  discovered[v] = 1;        /* starting vertex v */
  level[v] = l;

  //printf("processed vertex %d, level is %d\n",v,level[v]);
  p = g->edges[v];             /* get list of edges from v */
  
  while (p != NULL) {          /* loop over all edges */
    y = p->y;
    w = p->weight;



    if (discovered[y] == 0)
    {
      //printf("dfs on node %d\n", y);
      dfs(g,y,l+1,discovered,level);
    }


    p = p->next;

  }

}
 

Graph * create_reversed_graph(Graph * g)
{
  Graph * g_t = (Graph*)malloc(sizeof(Graph));
  /* initialize Graph fields */
  g_t->nvertices = g->nvertices;
  g_t->nedges = g->nedges;
  g_t->directed = 1;

  g_t->edges = (Edgenode**)malloc((g_t->nvertices+1)*sizeof(Edgenode*));
  for (int i=0;i<=g_t->nvertices;++i)
    g_t->edges[i] = NULL;

  

  for (int u = 1; u <= g->nvertices; u++)
  {
    Edgenode * p = g->edges[u];
    while(p)
    {
      int v = p->y;
      double w = p->weight;

      //Create transposed edges
      add_edge_to_graph(g_t,v,u,w);

      p = p->next;
    }
  }
  return g_t;
}

int graph_is_irreducible(Graph * g)
{
  // St1p 1: Mark all the vertices as not visited (For first DFS) 
  int discovered[g->nvertices+1];  /* which vertices have been found */
  int level[g->nvertices+1];       /* level of each node */   
  for (int i = 0; i <= g->nvertices; ++i)
  {
    discovered[i]=0;
    level[i]=-1;
  }        

  // Step 2: Do DFS traversal starting from first vertex. 
  dfs(g,1,0,discovered,level); 

  // If DFS traversal doesn’t visit all vertices, then return false. 
  for (int i = 1; i <= g->nvertices; i++) 
    if (discovered[i] == 0) 
      return 0;
 

  // Step 3: Create a reversed graph 
  Graph* g_t = create_reversed_graph(g); 

  // Step 4: Mark all the vertices as not visited (For second DFS) 
  for (int i = 0; i <= g->nvertices; ++i)
  {
    discovered[i]=0;
    level[i]=-1;
  }        

  // Step 5: Do DFS for reversed graph starting from first vertex. 
  // Staring Vertex must be same starting point of first DFS 
  dfs(g_t,1,0,discovered,level); 

  // If all vertices are not visited in second DFS, then return false 
  for (int i = 1; i <= g_t->nvertices; i++) 
    if (discovered[i] == 0) 
      return 0;

  return 1;
}


/* Returns number of lines read from file */
int read_file(char *filename, char text[][MAX_CHAR_PER_LINE])
{
  /*Reference: https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm*/
  FILE *fp;
  int linesCounter=0;

  /* opening file for reading */
  fp = fopen(filename , "r");

  while(fgets (text[linesCounter], MAX_CHAR_PER_LINE, fp)!=NULL )
  {
    /*printf("%s", text[linesCounter]);*/
    linesCounter++;
    
    if (linesCounter >= MAX_LINES)
    {
            break;
    }
  }
  /*printf("\n");*/
  fclose(fp);

  return linesCounter;
}



Graph * initialize_graph_from_file(char* filename)
{
  Graph * g = (Graph*) malloc(sizeof(Graph));

  char text[MAX_LINES][MAX_CHAR_PER_LINE];
  int nv = read_file(filename,text);
  

  //Create graph

  /* initialize Graph fields */
  g->nvertices = nv;
  int n_edges=0;

  g->directed = 1;

  g->edges = (Edgenode**)malloc((nv+1)*sizeof(Edgenode*));
  for (int i=0;i<=nv;++i)
    g->edges[i] = NULL;

  //For each node, create respective edges
  int v; // destiny node
  double w; // weight of edge
  for(int i = 0; i < nv; i++)
  {
    char *token = strtok(text[i], " "); 
    int u = atoi(token);
    //printf("Source_node: %d\n", u);

    while (token != NULL) 
    {  
      //Parsing to get destiny and probability
      token = strtok(NULL, "["); 
      if(token)
      {
        token = strtok(NULL, " ,");
        if(token)  
          v = atoi(token);

        token = strtok(NULL, "]");
        if(token)
        {
          w = atof(token);
          //printf("%d %f\n", v, w);

          add_edge_to_graph(g,u,v,w);

          n_edges++;

        }
      }
    } 
  }

  g->nedges = n_edges;
  
  return g;
}

//write a driver that takes the Markov matrix in the hw directory as input
//and calculates its period
int main ()
{
  Graph *g;
  //Edgenode *e;
  char *filename = "markov-graph.txt";
  
  g = initialize_graph_from_file(filename);

  int r = graph_is_irreducible(g);

  if (r)
    printf("Irreductible!\n");
  else
    printf("Not irreductible!\n");

}