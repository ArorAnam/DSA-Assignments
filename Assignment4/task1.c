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

//A structue to represent a queue
struct Queue 
{ 
    int front, rear, size; 
    unsigned capacity; 
    char *array; 
}; 
  
// function to create a queue of given capacity.  
// It initializes size of queue as 0 
struct Queue* createQueue(unsigned capacity) 
{ 
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue)); 
    queue->capacity = capacity; 
    queue->front = queue->size = 0;  
    queue->rear = capacity - 1;  // This is important, see the enqueue 
    queue->array = (char*) malloc(queue->capacity * sizeof(int)); 
    return queue; 
} 
  
// Queue is full when size becomes equal to the capacity  
int isFull(struct Queue* queue) 
{  return (queue->size == queue->capacity);  } 
  
// Queue is empty when size is 0 
int isEmpty(struct Queue* queue) 
{  return (queue->size == 0); } 
  
// Function to add an item to the queue.   
// It changes rear and size 
void enqueue(struct Queue* queue, char item) 
{ 
    if (isFull(queue)) 
        return; 
    queue->rear = (queue->rear + 1)%queue->capacity; 
    queue->array[queue->rear] = item; 
    queue->size = queue->size + 1; 
} 
  
// Function to remove an item from queue.  
// It changes front and size 
char dequeue(struct Queue *queue) 
{ 
    if (isEmpty(queue)) 
        return '0'; 
    char item = queue->array[queue->front]; 
    queue->front = (queue->front + 1)%queue->capacity; 
    queue->size = queue->size - 1; 
    return item; 
} 
  
// Function to get front of queue 
char front(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return '0'; 
    return queue->array[queue->front]; 
} 
  
// Function to get rear of queue 
char rear(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return '0'; 
    return queue->array[queue->rear]; 
} 
 
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

void BFS (struct AdjList *List, char s, int V) {
    printf("\nBFS traversal :: ");
    struct Queue *queue = createQueue(100);

    //mark all nodes as not visited
    int visited[V];
    for(int i=0;i<V;i++)
        visited[i] = 0;

    //mark the currrent node as visited and enqueue it
    int curr_pos = s - 'A';
    visited[curr_pos] = 1;
    enqueue(queue, s);

    while(!isEmpty(queue)) {
        //dequeue a node from queue
        s = front(queue);
        printf("%c\t",s);
        dequeue(queue);

        //Get all the adjacent vertices of the dequeued
        //bertex s. If a adjacent has not been visited,
        //then mark it visited and enqueue it
        int v = s - 'A';
        struct AdjListNode *temp = List[v].head;
        temp = temp->next;
        for(char c ; temp!=NULL ; ) {
            c = temp->data;
            int i = c - 'A';
            if(!visited[i]) {
                visited[i] = 1;
                enqueue(queue, c);
            }
            temp = temp->next;
        }
    }
    printf("\n");
}

void DFSUtil(int v, int visited[], struct AdjList *List) {
    //Mark the node as visited and print it
    visited[v] = 1;
    printf("%c\t", List[v].head->data);

    //Recur for all the vertices adjacent to this vertex
    struct AdjListNode *temp = List[v].head;
    temp = temp->next;
    for(char c ; temp!=NULL; ) {
        c = temp->data;
        int i = c - 'A';
        if(!visited[i]) {
            DFSUtil(i, visited, List);
        }
        temp = temp->next;
    }
}

void DFS(struct AdjList *List, int V) {
    printf("\nDFS traversal :: ");

    //Mark all the vertices as not visited
    int visited[V];
    for(int i=0;i<V;i++)
        visited[i] = 0;

    //call the resursive DFS function
    //starting from all vertices one by one
    for(int i=0;i<V;i++) {
        if(visited[i] == 0)
            DFSUtil(i, visited, List);
    }
    
    printf("\n");
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
  
    BFS(List, 'A', V);

    DFS(List, V);

    return 0; 
} 
