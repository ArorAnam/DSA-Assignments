#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_BUFFER 256

// A structure to represent an adjacency list node 
struct AdjListNode
{
    int StopId;
    char Name[MAX_BUFFER];
    char Latitude[MAX_BUFFER] ;
	char Longitude[MAX_BUFFER]; 
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

struct FileEdges {
	int src;
	int dest;
	int weight;
};

// The CSV parser
int
next_field( FILE *f, char *buf, int max ) {
	int i=0, end=0, quoted=0;
	
	for(;;) {
		// fetch the next character from file		
		buf[i] = fgetc(f);
		// if we encounter quotes then flip our state and immediately fetch next char
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		// end of field on comma if we're not inside quotes
		if(buf[i]==',' && !quoted) { break; }
		// end record on newline or end of file
		if(feof(f) || buf[i]=='\n') { end=1; break; } 
		// truncate fields that would overflow the buffer
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
	return end; // flag stating whether or not this is end of the line
}

// Stuff to make life a bit neater in main
struct AdjListNode*
fetchData (FILE *csv) {
	char buf[MAX_BUFFER];
	struct AdjListNode *newNode = 
		(struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	
	next_field( csv, buf, MAX_BUFFER );      // load id into buffer as string
	newNode->StopId = atoi(buf);

	next_field( csv, newNode->Name, MAX_BUFFER );  // name and type are just strings so read
	next_field( csv, newNode->Latitude, MAX_BUFFER ); // those directly into the struct
	next_field( csv, newNode->Longitude, MAX_BUFFER );
	
	newNode->next = NULL;
	newNode->weight = -1;

	return newNode;
}

struct FileEdges*
fetchEdge (FILE *csv) {
	char buf[MAX_BUFFER];
	struct FileEdges *newNode = 
		(struct FileEdges*)malloc(sizeof(struct FileEdges));
	
	next_field( csv, buf, MAX_BUFFER );     
	newNode->src = atoi(buf);

	next_field( csv, buf, MAX_BUFFER );     
	newNode->dest = atoi(buf);

	next_field( csv, buf, MAX_BUFFER );     
	newNode->weight = atoi(buf);

	return newNode;
}

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

void readVertices(struct Graph *graph) {
	FILE *f;
	f = fopen("vertices.csv", "r");
	if(!f) { 
		printf("unable to open\n"); 
		return ; 
	}

	fetchData(f);

	int i=0;
	while(!feof(f)) {
		graph->array[i].head = fetchData(f);
		i++;
	}
}

void addEdge
(struct Graph *graph, struct FileEdges *e) {
    //add this node at the end of the list for src
	// struct AdjListNode *newNode = 
	// 	(struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	struct AdjListNode *temp = graph->array[e->dest].head;
	printf("\n%d", temp->StopId); 
	// strcpy(newNode->Name, temp->Name);
	// newNode->StopId = temp->StopId;
	// strcpy(newNode->Latitude, temp->Latitude);
	// strcpy(newNode->Longitude, temp->Longitude);
	// newNode->weight = e->weight;
	// newNode->next = NULL;

    // struct AdjListNode *temp = graph->array[e->src].head;
    // while(temp->next)
    //     temp = temp->next;
    // temp->next = newNode;

    // //since graph is undirected, add an edge from desc to source also
	// struct AdjListNode *newNode2 = 
	// 	(struct AdjListNode*) malloc(sizeof(struct AdjListNode)); 
	// strcpy(newNode2->Name, graph->array[e->src].head->Name);
	// newNode2->StopId = graph->array[e->src].head->StopId;
	// strcpy(newNode2->Latitude, graph->array[e->src].head->Latitude);
	// strcpy(newNode2->Longitude, graph->array[e->src].head->Longitude);
	// newNode2->weight = e->weight;
	// newNode2->next = NULL;

    // temp = graph->array[e->dest].head;
    // while(temp->next)
    //     temp = temp->next;
    // temp->next = newNode2;
}


void edgeCal(struct Graph *graph) {
	FILE *p;
	p = fopen("edges.csv", "r");
	if(!p) { 
		printf("unable to open\n"); 
		return ; 
	}

	fetchEdge(p);

	struct FileEdges *temp;
	while(!feof(p)) {
		temp = fetchEdge(p);
		//now put data into graph array;
		addEdge(graph, temp);
	}
}

void printGraph(struct Graph *graph) 
{ 
    int v; 
    for (v = 0; v < graph->V; v++) 
    { 
        struct AdjListNode* pCrawl = graph->array[v].head; 
        printf("\n Adjacency list of vertex %s\n head ", 
                graph->array[v].head->Name); 
        while (pCrawl) 
        { 
            printf("-> %s", pCrawl->Name); 
            pCrawl = pCrawl->next; 
        } 
        printf("\n"); 
    } 
} 

int main() {
	int V = 4806;
    struct Graph *graph = createGraph(V);
    if(graph)
        readVertices(graph);
    else
        printf("memory failure!\n");

	edgeCal(graph);

	//printGraph(graph);

    return 0;
}
