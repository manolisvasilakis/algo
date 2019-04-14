#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int N, counter = 0, local_count;

// A structure to represent a node in an integer list
struct IntNode {
    int node;
    struct IntNode* next;
};

// A structure to represent a list of integers
struct IntList {
    struct IntNode *head; // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
    int V;
    struct IntList* array;
};

// A utility function to create a new adjacency list node
struct IntNode* newIntNode(int dest) {
    struct IntNode* newNode = (struct IntNode*) malloc(sizeof(struct IntNode));
    newNode->node = dest;
    newNode->next = NULL;
    return newNode;
}

void push(struct IntList queue, struct IntNode* node) {
    node->next = queue.head;
    queue.head = node;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph (int V) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists. Size of array will be V
    graph->array = (struct IntList*) malloc(V * sizeof(struct IntList));

    // Initialize each adjacency list as empty by making head as NULL
    for (int i=0; i<V; ++i) graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to our graph and create all the layered edges
void addEdge (struct Graph* graph, int src, int dest) {

    // Add an edge from src to dest. A new node is added to the adjacency
    // list of src. The node is added at the beggining.
    struct IntNode* newNode = newIntNode(dest);
    push(graph->array[src], newNode);
}

DFS_Util(int s, struct Graph *g, bool visited[])
{
    // Mark the current node as visited and
    // print it
    visited[s] = true;
    local_count++;

    // Recur for all the vertices adjacent
    // to this vertex
    for (i = g->array[s].head; i != NULL; i = i->next)
        if (!visited[i->node])
            DFSUtil(i->node, visited);
}

void DFS(int s, struct Graph *g)
{
    int V = g->V;
    local_count = 0;
    // Mark all the vertices as not visited
    bool *visited = (bool *) malloc(V * sizeof(bool));
    for(int i = 0; i < V; i++)
        visited[i] = false;

    DFS_Util(s, g, visited);
    if (local_count == V) counter++;
}

int main()
{
    int i, j, loss, src;
    scanf("%d", &N);

    // Create a graph given in the above diagram, Winner -> loser
    struct Graph* graph = createGraph(N);

    for (i=0; i<N; i++) {
        scanf("%d", &loss);
        for (j=0; j<loss; j++) {
            scanf("%d", &src);
            addEdge(graph, src-1, i);
        }
    }

    for (i=0; i<N; i++) {
        DFS(i, g);
    }

    printf("%d\n", counter);
    return 0;
}
