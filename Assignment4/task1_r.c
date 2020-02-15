// A C Program to demonstrate adjacency list  
// representation of graphs 
#include <stdio.h> 
#include <stdlib.h> 
  
// A structure to represent an adjacency list node 
struct AdjListNode
{ 
    char data; 
    struct AdjListNode *next; 
}; 
  
// A structure to represent an adjacency list 
struct AdjList 
{ 
    struct AdjListNode *head;  
};  
  
// A utility function to create a new adjacency list node 
struct AdjListNode* newAdjListNode(char data) 
{ 
    struct AdjListNode* newNode = 
     (struct AdjListNode*) malloc(sizeof(struct AdjListNode)); 
    newNode->data = data; 
    newNode->next = NULL; 
    return newNode; 
} 
  
// A utility function that creates a graph of V vertices 
struct AdjList* createList(int V) {
    struct AdjList *List = 
        (struct AdjList*)malloc(V * sizeof(struct AdjList));

    int i; 
    for (i = 0; i < V; i++) 
        List[i].head = NULL; 
  
    return List; 

}

//Initializes the heads of the array of AdjList
void initializeList (struct AdjList *array, int V) {
    char temp = 'A';
    for(int i=0; i<V; i++) {
        array[i].head = newAdjListNode(temp);
        temp++;
    }
}
  
// Adds an edge to an undirected graph 
void addEdge(struct AdjList* array, char src, char dest) 
{ 
    // Add an edge from src to data.  A new node is  
    // added to the adjacency list of src.  The node
    // is added at the end of the list
    struct AdjListNode* newNode = newAdjListNode(dest);
    char ini = 'A';
    int v = src - ini;
    printf("\n%d", v);

    struct AdjListNode *temp = array[v].head; 
    while(temp->next) 
        temp = temp->next;    
    temp->next = newNode;  
} 
  
// A utility function to print the adjacency list  
// representation of graph 
void printGraph(struct AdjList *array, int V) 
{ 
    int v; 
    for (v = 0; v < V; v++) 
    { 
        struct AdjListNode* pCrawl = array[v].head; 
        printf("\n Adjacency list of vertex %c\n head ", array[v].head->data); 
        while (pCrawl) 
        { 
            printf("-> %c", pCrawl->data); 
            pCrawl = pCrawl->next; 
        } 
        printf("\n"); 
    } 
} 
  
// Driver program to test above functions 
int main() 
{ 
    // create the graph given in above fugure 
    int V = 6; 
    // struct Graph* graph = createGraph(V); 
    // initializeList(graph);
    struct AdjList *List = createList(V);

    initializeList(List, V);

    addEdge(List, 'A', 'B');
    addEdge(List, 'A', 'D');
    addEdge(List, 'A', 'E');
    addEdge(List, 'B', 'C');
    addEdge(List, 'B', 'D');
    addEdge(List, 'C', 'B');
    addEdge(List, 'C', 'D');
    addEdge(List, 'D', 'F');
    addEdge(List, 'E', 'D');
    addEdge(List, 'F', 'C'); 
    // print the adjacency list representation of the above graph 
    printGraph(List, V); 
  
    return 0; 
} 
