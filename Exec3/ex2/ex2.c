// CHECK HERE : https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-algorithm-greedy-algo-2/
// and HERE   : https://www.geeksforgeeks.org/bridge-in-a-graph/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// a structure to represent a weighted edge in graph
struct Edge {
    long src, dst;
    int weight;
};

// A structure to represent a subset for union-find
struct subset {
    long parent;
    long rank;
};

// A structure to represent neighbors list
struct adjlist {
    int weight;
    long dst;
    struct adjlist *next;
};

// A structure to represent our graph
struct Graph {
    int visited;
    long disc, low, parent;
    struct adjlist *neighbors;
};

struct Edge *edge;              // edo kratame akmes gia tajinomhsh
struct subset *subsets;         // union - find
long always = 0, time;
int cost;
struct Graph *graph;            // our graph

// A utility function to find set of an element i (uses path compression technique)
long find(long i) {
    // find root and make root as parent of i (path compression)
    if (subsets[i].parent != i) subsets[i].parent = find(subsets[i].parent);

    return subsets[i].parent;
}

// A function that does union of two sets of x and y (uses union by rank)
void Union(long x, long y) {
    long xroot = find(x);
    long yroot = find(y);

    // do nothing if already united
    if (xroot == yroot) return;

    // Attach smaller rank tree under root of high rank tree (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank) subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank) subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and increment its rank by one
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Compare two edges according to their weights. Used in qsort() for sorting an array of edges
int myComp(const void* a, const void* b) {
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    return a1->weight > b1->weight;
}

// dfs check if bridge
void dfs_bridge(long u) {

    long v;
    struct adjlist *temp = graph[u].neighbors;

    graph[u].visited = 1;
    time++;
    graph[u].disc = time;
    graph[u].low = time;

    while (temp != NULL) {

        v = temp->dst;

        if ((graph[v].visited) == 0) {
            graph[v].parent = u;

            dfs_bridge(v);

            if (graph[u].low > graph[v].low) graph[u].low = graph[v].low;

            if ((graph[v].low > graph[u].disc) && (temp->weight == cost)) always++;
        }
        else if (v != graph[u].parent) {
            if (graph[u].low > graph[v].disc) graph[u].low = graph[v].disc;
        }

        temp=temp->next;
    }
    return;
}

int main() {
    long N; // Number of vertices in graph
    long M; // Number of edges in graph
    long i, j, x, y, src, dst, original, some = 0, none = 0;
    struct adjlist *temp;

    scanf("%li %li", &N, &M);

    edge = (struct Edge*) malloc(M * sizeof(struct Edge));
    if (!edge) {
        printf("ERROR WITH MALLOC\n");
        exit(1);
    }

    // add edges
    for(i=0; i<M; i++) {
        scanf("%li %li %d", &src, &dst, &cost);
        edge[i].src = src - 1;
        edge[i].dst = dst - 1;
        edge[i].weight = cost;
    }

    // sort edges
    qsort(edge, M, sizeof(struct Edge), myComp);

    // Allocate memory for creating N subsets
    subsets = (struct subset*) malloc(N * sizeof(struct subset));
    if (!subsets) {
        printf("ERROR WITH MALLOC\n");
        exit(1);
    }

    // Allocate memory for our graph
    graph = (struct Graph*) malloc(N * sizeof(struct Graph));
    if (!graph) {
        printf("ERROR WITH MALLC\n");
        exit(1);
    }

    // Create N subsets with single elements and other initialisations
    for (i=0; i<N; i++) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
        graph[i].neighbors = NULL;
    }

    // process edges by grouping them with the same weight
    i = 0;
    while (i < M) {

        time = 0;
        cost = edge[i].weight;
        original = i;
        temp = NULL;

        while (edge[i].weight == cost) {
            x = find(edge[i].src);
            y = find(edge[i].dst);

            // not needed edge, union does not change anything
            if (x == y) none++;
            else {
                temp = (struct adjlist *) malloc(sizeof(struct adjlist));
                if (!temp) {
                    printf("ERROR WITH MALLOC\n");
                    exit(1);
                }

                temp->weight = cost;
                temp->dst = edge[i].dst;
                temp->next = graph[edge[i].src].neighbors;
                graph[edge[i].src].neighbors = temp;

                temp = (struct adjlist *) malloc(sizeof(struct adjlist));
                if (!temp) {
                    printf("ERROR WITH MALLOC\n");
                    exit(1);
                }

                temp->weight = cost;
                temp->dst = edge[i].src;
                temp->next = graph[edge[i].dst].neighbors;
                graph[edge[i].dst].neighbors = temp;
            }

            i++;
        }

        for (j=0; j<N; j++) {
            if (graph[j].neighbors != NULL) {
                graph[j].visited = 0;
                graph[j].disc = 0;
                graph[j].low = 0;
                graph[j].parent = 0;
            }
        }

        for (j=0; j<N; j++) {
            if ((graph[j].neighbors != NULL) && (graph[j].visited == 0))
                dfs_bridge(j);
        }

        // add all the new edges in the graph
        for (j=original; j<i; j++) Union(edge[j].src, edge[j].dst);
    }

    some = M - none - always;
    printf("%li\n%li\n%li\n", always, none, some);

    return 0;
}
