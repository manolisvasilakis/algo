// C / C++ program for Dijkstra's shortest path algorithm for adjacency
// list representation of graph
// CHECK HERE : https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/
// k graphs with k -> (k-1) edges for reverse direction

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// A structure to represent a node in adjacency list
struct AdjListNode {
    int dest;
    long long weight;
    struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList {
    struct AdjListNode *head; // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
// Number of maximum accepted errors will be K
struct Graph {
    int V;
    int K;
    struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, long long weight) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices and max K errors
struct Graph* createGraph (int V, int K) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    graph->K = K;

    // Create an array of adjacency lists. Size of array will be V * (K+1)
    graph->array = (struct AdjList*) malloc(V * (K+1) * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < (V*(K+1)); ++i) graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to our graph and create all the layered edges
void addEdgeMultipleTimes (struct Graph* graph, int src, int dest, long long weight) {
    int i;
    int K = graph->K;
    int V = graph->V;
    struct AdjListNode* newNode;

    // Add an edge from src to dest. A new node is added to the adjacency
    // list of src. The node is added at the beggining.
    // src+i | dest+i because edge is added to all layers.
    for (i = 0; i <= K; i++) {
        newNode = newAdjListNode(dest + (i*V), weight);
        newNode->next = graph->array[src + (i*V)].head;
        graph->array[src + (i*V)].head = newNode;
    }

    // Add reverse edges from k to (k-1) if reverse edge is used.
    // this is the reason for src+i-i | dest+i
    for (i = K; i > 0; i--) {
        newNode = newAdjListNode(src + ((i-1)*V), weight);
        newNode->next = graph->array[dest + (i*V)].head;
        graph->array[dest + (i*V)].head = newNode;
    }
}

// Structure to represent a min heap node
struct MinHeapNode {
    int v;
    long long dist;
};

// Structure to represent a min heap
struct MinHeap {
    int size;       // Number of heap nodes present currently
    int capacity;   // Capacity of min heap
    int *pos;       // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, long long dist) {
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *) malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx) {
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

    if (smallest != idx) {
        // The nodes to be swapped in min heap
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if the given minHeap is empty or not
bool isEmpty(struct MinHeap* minHeap) {
    return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Function to decrease dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, long long dist) {
    // Get the index of v in heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    // Travel up while the complete tree is not heapified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v) {
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

// The main function that calulates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Graph* graph, int src, int errors, int dst) {
    int V = (1 + errors) * graph->V ;   // Get the number of vertices in graph that will be useful
    long long dist[V];                  // dist values used to pick minimum weight edge in cut

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices that we will use. dist value of all vertices
    for (int v = 0; v < V; ++v) {
        dist[v] = LLONG_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap)) {
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number
        if (dist[u] == LLONG_MAX) break;

        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && pCrawl->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->weight;

                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // print the calculated shortest distance
    long long min = LLONG_MAX;
    for (int j = 0; j <= errors; j++) {
        if (dist[dst + (j*(graph->V))] < min) min = dist[dst + (j*(graph->V))];
    }

    if (min == LLONG_MAX) printf("IMPOSSIBLE\n");
    else printf("%lld\n", min);
}


int main() {

    int N, M, K, Q;
    int i, temp1, temp2, errors, start;
    long long dist;

    // read input parameters, K = 0, ..., 10
    scanf("%d %d %d %d", &N, &M, &K, &Q);

    // create the graph given in input (all the layers)
    struct Graph* graph = createGraph(N, K);
    for (i=0; i<M; i++) {
        scanf("%d %d %lld", &temp1, &temp2, &dist);
        addEdgeMultipleTimes(graph, temp1-1, temp2-1, dist);
    }

    for (i=0; i<Q; i++) {
        scanf("%d %d %d", &temp1, &temp2, &errors);
        start = temp1-1 + (errors * N);

        dijkstra(graph, start, errors, temp2-1);
    }

    return 0;
}
