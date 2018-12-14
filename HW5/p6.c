#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <limits.h>

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


void add_edge_to_graph(Graph * g, int u, int v, double w)
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
          //printf("%d %d %f\n", u, v, w);
          add_edge_to_graph(g,u,v,w);

          n_edges++;

        }
      }
    } 
  }

  g->nedges = n_edges;
  
  return g;
}


double * generate_CDF(Graph* g, int a)
{
  double * cdf = (double*) malloc((g->nvertices+1)*sizeof(double));
  for (int i = 0; i <= g->nvertices; ++i)
    cdf[i]=0;

  Edgenode * p =g->edges[a];
  for (int i = 1; i <= g->nvertices; ++i)
  {
    cdf[i]=cdf[i-1];
    if(p && p->y == i)
    {
      //printf("Edge from %d to %d with weight %f\n", a, p->y, p->weight);
      cdf[i] = cdf[i] + p->weight;
      p=p->next;
    }
    //printf("cdf[%d]=%f\n", i,cdf[i]);
  }
  return cdf;
}


int random_jump_in_chain(Graph * g, int a)
{
  //Step 1: Get the row of the matrix asociated to transitions from node a
  //and generate cdf - accumulative vector of probabilities of transitions
  double * cdf = generate_CDF(g,a);

  //Step 2: Generate random number u ~ U(0,1)
  double u = (double)rand() / INT_MAX;


//  printf("u is %f\n", u);

  //Step 3: Find the first node such that CDF(node) > u. Return that node
  for (int i = 1; i <= g->nvertices; ++i)
  {
    //printf("cdf[%d] = %f\n", i, cdf[i]);
    if(cdf[i]>=u)
      return i;
  }
  //We shouldnt get to this point if rows sum up to 1, but in some cases they dont.
  return g->nvertices;
}

//Based on randomization for n steps
void calculate_stationary_probabilities(Graph* g, int n)
{
  /*so start from any node (a) in the graph.
  get the row of the matrix asociated with transitions from (a).
  transform that row (pdf) to (cdf), so as to generate a random jump to a
  new state (b). add +1 to your register of times you have visited node b.
  do this a lot of times. is that it?*/

  double * visits_counter = (double*)malloc((g->nvertices+1)*sizeof(double));
  for (int i = 0; i <= g->nvertices; ++i)
    visits_counter[i]=0;

  //Step 0: Choose a random node from where to start, node a
  time_t t; 
  //Intializes random number generator
  srand((unsigned) time(&t));
  int a = 1;//rand() % g->nvertices +1;
  int b;
  for (int i = 0; i < n; ++i)
  {
    //Get random next step from a in the chain
    b = random_jump_in_chain(g,a);
    visits_counter[b]++;
    //printf("jumping from %d to %d\n", a,b);
    a = b;
  }

  for (int i = 1; i <= g->nvertices; ++i)
  {
    printf("Pi_%d: %f, ", i, visits_counter[i]/n);
  }
}

//Calculate stationary probabilities from randomization
int main ()
{
  Graph *g;
  //Edgenode *e;
  char *filename = "markov-graph.txt";
  
  g = initialize_graph_from_file(filename);
  
  calculate_stationary_probabilities(g,1000000);

}