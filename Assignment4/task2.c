#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// A structure to represent an adjacency list node 
struct AdjListNode
{ 
    char data; 
    int weight;
    struct AdjListNode *next; 
}; 
  
// A structure to represent an adjacency list 
struct AdjList 
{ 
    struct AdjListNode *head;  
}; 

//A structure to represent a graph. A graph is an array
//of adjancency lists. Size of the array will be V
struct Graph
{
    int V;
    struct AdjList *array;
};

struct prevNode {
    char data;
};
  
// A utility function to create a new adjacency list node 
struct AdjListNode* newAdjListNode(char data, int weight) 
{ 
    struct AdjListNode* newNode = 
     (struct AdjListNode*) malloc(sizeof(struct AdjListNode)); 
    newNode->data = data;
    newNode->weight = weight; 
    newNode->next = NULL; 
    return newNode; 
} 

//A Utility function to create a graph of V vertices
struct Graph* createGraph(int V) {
    struct Graph *graph = 
        (struct Graph*)malloc(sizeof(struct Graph));
    if(graph) {
        graph->V = V;

    //create an array of adjancency lists. Size of the array
    //will be V
    graph->array = 
        (struct AdjList*)malloc(V*sizeof(struct AdjList));

    //Inistialize each adjancency list as empty
    for(int i=0;i<V;i++)
        graph->array[i].head = NULL;

    return graph;
    }
    else
        return NULL; 
}

void initializeList (struct Graph *graph) {
    char temp = 'A';
    for(int i=0; i<graph->V; i++) {
        graph->array[i].head = newAdjListNode(temp,-1);
        temp++;
    }
}

void addEdge
(struct Graph *graph, char src, char dest, int weight) {
    struct AdjListNode *newNode = newAdjListNode(dest, weight);

    //add this node at the end of the list for src
    int v = src - 'A';
    struct AdjListNode *temp = graph->array[v].head;
    while(temp->next)
        temp = temp->next;
    temp->next = newNode;

    //since graph is undirected, add an edge from desc to source also
    newNode = newAdjListNode(src,  weight);
    v = dest - 'A';
    temp = graph->array[v].head;
    while(temp->next)
        temp = temp->next;
    temp->next = newNode;
}

void destroyHead (struct Graph **graph) {
    for(int i=0;i<(*graph)->V;i++) {
        struct AdjListNode *temp = 
            (*graph)->array[i].head;
        (*graph)->array[i].head = 
            (*graph)->array[i].head->next;
        free(temp);
    }
}

//structure to represent a min heap node
struct MinHeapNode
{
    char v;
    int dist;
};

//structure to represent a min heap
struct MinHeap
{
    int size;   //no. of min heaps present currently
    int capacity;   //capacity of min heap
    int *pos;   //needed for decreaseKey()
    struct MinHeapNode **array;
};

//a utility fn to create a new min heap node
struct MinHeapNode* newMinHeapNode(char v, int dist) {
    struct MinHeapNode *minHeapNode = 
        (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

//a utility function to create a min heap
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap *minHeap =
        (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int)); 
    minHeap->size = 0; 
    minHeap->capacity = capacity; 
    minHeap->array = 
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*)); 
    return minHeap; 
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) 
{ 
    struct MinHeapNode *t = *a; 
    *a = *b; 
    *b = t; 
} 

// A standard function to heapify at given idx 
// This function also updates position of nodes when they are swapped. 
// Position is needed for decreaseKey() 
void minHeapify(struct MinHeap* minHeap, int idx) 
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
        struct MinHeapNode *smallestNode = minHeap->array[smallest]; 
        struct MinHeapNode *idxNode = minHeap->array[idx]; 
  
        // Swap positions
        int j = smallestNode->v - 'A'; 
        minHeap->pos[j] = idx;
        j = idxNode->v - 'A';
        minHeap->pos[j] = smallest; 
  
        // Swap nodes 
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); 
  
        minHeapify(minHeap, smallest); 
    } 
} 
  
// A utility function to check if the given minHeap is ampty or not 
int isEmpty(struct MinHeap* minHeap) 
{ 
    return minHeap->size == 0; 
} 
  
// Standard function to extract minimum node from heap 
struct MinHeapNode* extractMin(struct MinHeap* minHeap) 
{ 
    if (isEmpty(minHeap)) 
        return NULL; 
  
    // Store the root node 
    struct MinHeapNode *root = minHeap->array[0]; 
  
    // Replace root node with last node 
    struct MinHeapNode *lastNode = minHeap->array[minHeap->size - 1]; 
    minHeap->array[0] = lastNode; 
  
    // Update position of last node
    int j = root->v - 'A'; 
    minHeap->pos[j] = minHeap->size-1;
    j = lastNode->v - 'A'; 
    minHeap->pos[j] = 0; 
  
    // Reduce heap size and heapify root 
    --minHeap->size; 
    minHeapify(minHeap, 0); 
  
    return root; 
} 

// Function to decreasy dist value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void decreaseKey(struct MinHeap* minHeap, char v, int dist) 
{ 
    // Get the index of v in  heap array 
    int j = v - 'A';
    int i = minHeap->pos[j]; 
  
    // Get the node and update its dist value 
    minHeap->array[i]->dist = dist; 
  
    // Travel up while the complete tree is not hepified. 
    // This is a O(Logn) loop 
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) 
    { 
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v - 'A'] = (i-1)/2; 
        minHeap->pos[minHeap->array[(i-1)/2]->v - 'A'] = i; 
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]); 
  
        // move to parent index 
        i = (i - 1) / 2; 
    } 
} 

// A utility function to check if a given vertex 
// 'v' is in min heap or not 
int isInMinHeap(struct MinHeap *minHeap, int v) 
{ 
   if (minHeap->pos[v] < minHeap->size) 
     return 1; 
   return 0; 
} 

// A utility function used to print the solution 
void printArr(int dist[], int n) 
{ 
    printf("\n\nShortest Distance from Source A :: \n"); 
    for (int i = 1; i < n; ++i) 
        printf("%c \t\t %d\n", i+'A', dist[i]); 
} 

void printPath(struct prevNode *prev, int V) {
    int i=1;
    while(i<V) {
        printf("\nShortest Path for %c :: %c",'A'+i, 'A'+i);
        char temp = prev[i].data;
        while(temp!='A') {
            printf("<-%c", temp);
            int j = temp - 'A';
            temp = prev[j].data;
        }
        printf("<-A");
        i++;
    }
    printf("\n");
}

void dijkstra(struct Graph *graph, char src) {
    int V = graph->V;
    int dist[V];    //dist values used to pickup minimum weight edge in cut
    
    struct prevNode *prev = (struct prevNode*)malloc(V * sizeof(struct prevNode));

    //minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);
    
    //initialize min heap with all vertices.
    //dist values of all vertices
    for(int v=0;v<V;v++) {
        dist[v] = 1000;
        char c = 'A' + v;
        minHeap->array[v] = newMinHeapNode(c, dist[v]);
        minHeap->pos[v] = v;
    }

    //make dist value of src vertex as 0 so that it is
    //extracted first
    int v = src-'A';
    minHeap->array[v] = newMinHeapNode(src, dist[v]);
    minHeap->pos[v]   = v;
    dist[v] = 0;
    decreaseKey(minHeap, src, dist[v]);

    printf("\nPermanent path :: ");

    //initialize size of min heap is equal to V
    minHeap->size = V;

    //in the foloowing loop min heap contains all nodes
    //whose shortest distance is not yet finalised
    while(!isEmpty(minHeap)) {
        //extract the vertex with minimum distance value
        struct MinHeapNode *minHeapNode = 
            extractMin(minHeap);
        printf("%c, ", minHeapNode->v);
        int u = minHeapNode->v - 'A';   //store the extracted vertex number

        //Traverse through all adjacent vertices of u
        //(the extracted vertex) and update their distance
        //values
        struct AdjListNode *pCrawl = graph->array[u].head;
        struct AdjListNode *pCrawl2 = pCrawl->next;
        while(pCrawl2!=NULL) {
            int v = pCrawl2->data - 'A';

            //if shortest distance to v is not finalised yet,
            //distance to v through u is less than its previously
            //calculated distance
            if(isInMinHeap(minHeap, v) && dist[u]!=1000 &&
                    pCrawl2->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl2->weight;

                //update distance value in min heap also
                char l = 'A' + v;
                decreaseKey(minHeap, l, dist[v]);

                prev[v].data = u + 'A';
            }
            pCrawl2 = pCrawl2->next;
        }
    }

    //print the calculated shortest distances
    printArr(dist, V);
    printPath(prev, V);
}

void printGraph(struct Graph *graph) 
{ 
    int v; 
    for (v = 0; v < graph->V; v++) 
    { 
        struct AdjListNode* pCrawl = graph->array[v].head; 
        printf("\n Adjacency list of vertex %c\n head ", 
                graph->array[v].head->data); 
        while (pCrawl) 
        { 
            printf("-> %c", pCrawl->data); 
            pCrawl = pCrawl->next; 
        } 
        printf("\n"); 
    } 
} 

int main() {
    int V = 7;
    struct Graph *graph = createGraph(V);
    if(graph)
        initializeList(graph);
    else
        return 0;

    addEdge(graph, 'A', 'B', 1);
    addEdge(graph, 'A', 'C', 3);
    addEdge(graph, 'B', 'C', 1);
    addEdge(graph, 'B', 'D', 7);
    addEdge(graph, 'B', 'E', 5);
    addEdge(graph, 'B', 'G', 2);
    addEdge(graph, 'C', 'D', 9);
    addEdge(graph, 'C', 'E', 3);
    addEdge(graph, 'D', 'E', 2);
    addEdge(graph, 'D', 'G', 12);
    addEdge(graph, 'D', 'F', 1);
    addEdge(graph, 'E', 'F', 2);
    addEdge(graph, 'F', 'A', 10);

    // destroyHead(&graph);
    //printGraph(graph);

    dijkstra(graph, 'A');

    return 0;
}