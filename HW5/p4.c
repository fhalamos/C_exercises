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




void dfs(Graph *g, int v, int l, int* discovered, int* level){
  Edgenode *p;
  int y;
  
  discovered[v] = 1;        /* starting vertex v */
  level[v] = l;

  //printf("processed vertex %d, level is %d\n",v,level[v]);
  p = g->edges[v];             /* get list of edges from v */
  
  while (p != NULL) {          /* loop over all edges */
    y = p->y;
    if (discovered[y] == 0)
      dfs(g,y,l+1,discovered,level);

    p = p->next;
  }

}

// Function to return gcd of a and b 
int gcd(int a, int b) 
{ 
    if (a == 0) 
        return b; 
    return gcd(b % a, a); 
} 
  
// Function to find gcd of array of 
// numbers 
int findGCD(int arr[], int n) 
{ 
    int result = arr[0]; 
    for (int i = 1; i < n; i++) 
        result = gcd(arr[i], result); 
  
    return result; 
} 

//calculates the period of a directed graph G
void graph_period(Graph * g)
{
  int k_e [g->nedges];
  int edge_counter=0;

  int discovered[g->nvertices+1];  /* which vertices have been found */
  int level[g->nvertices+1];       /* level of each node */           

  dfs(g,1,0, discovered, level);

  for (int i = 1; i <= g->nvertices; ++i)
  {
    Edgenode * p = g->edges[i];
    while(p)
    {
      k_e[edge_counter] = abs(level[p->y]-level[i]-1);
      //printf("k_e[%d] = %d\n",edge_counter,k_e[edge_counter]);
      edge_counter++;
      p=p->next;
    }
  }

  printf("GCD: %d\n", findGCD(k_e,g->nedges));

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

  graph_period(g);


}