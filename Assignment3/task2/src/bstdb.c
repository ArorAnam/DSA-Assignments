#include "bstdb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Write your submission in this file
//
// A main function and some profiling tools have already been set up to test
// your code in the task2.c file. All you need to do is fill out this file
// with an appropriate Binary Search Tree implementation.
//
// The input data will be books. A book is comprised of a title and a word
// count. You should store both these values in the tree along with a unique
// integer ID which you will generate.
//
// We are aiming for speed here. A BST based database should be orders of
// magnitude faster than a linked list implementation if the BST is written
// correctly.
//
// We have provided an example implementation of what a linked list based
// solution to this problem might look like in the db/listdb.c file. If you
// are struggling to understand the problem or what one of the functions
// below ought to do, consider looking at that file to see if it helps your
// understanding.
//
// There are 6 functions you need to look at. Each is provided with a comment
// which explains how it should behave. The functions are:
//
//  + bstdb_init
//  + bstdb_add
//  + bstdb_get_word_count
//  + bstdb_get_name
//  + bstdb_stat
//  + bstdb_quit
//
// Do not rename these functions or change their arguments/return types.
// Otherwise the profiler will not be able to find them. If you think you
// need more functionality than what is provided by these 6 functions, you
// may write additional functions in this file.

struct Node {
	int doc_id;
	char *name;
	int word_count;

	struct Node *left;
	struct Node *right;

	int height;
};

int  	     next_id;
struct Node *bst_root;

int num_comps;
int num_searches;

int height(struct Node* N) {
	if(N == NULL)
		return 0;
	return N->height;
}

int max(int a, int b) {
	return (a>b)? a : b;
}

void free_tree_node (struct Node *root) {
	if(root == NULL)
        return;
    
    //first delete both subtrees
    free_tree_node(root->left);
    free_tree_node(root->right);

    //then delete the node
    free(root);
}

struct Node * 
search_item (struct Node *root, int doc_id) {
    //root is null
    if(root == NULL)
        return root;

	num_comps++;
    if(root->doc_id == doc_id)
        return root;

    else if(root->doc_id < doc_id)
        return search_item(root->right, doc_id);

    else 
        return search_item(root->left, doc_id);
}

struct Node *
find_document (int doc_id ) {
	// struct Node *p = bst_root;

	//count the number of times this function was called so we can get
	//average number of nodes traversed for each query
	num_searches++;

	struct Node *temp = search_item(bst_root, doc_id);

	return temp;		
}

struct Node* newNode(int doc_id, char *name, int word_count) 
{ 
	size_t len_name;

    struct Node *temp = (struct Node *)malloc(sizeof(struct Node)); 
    if(temp) {
		temp->doc_id = doc_id;
		temp->word_count = word_count;
		temp->left = temp->right = NULL;
		temp->height = 1; //new node is intially added at leaf

		//Aloccate memory to store the book name and
		//copy the string into new tree node
		len_name = strlen(name)+1;
		temp->name = calloc(len_name, sizeof(char));
		if(temp->name) {
			//if calloc was successful
			strcpy(temp->name, name);
		} else {
			//if calloc failed, release memory
			free(temp);
			temp = NULL;
		}
	}

    return temp; 
} 

//right rotate the subtree rooted with y
struct Node* rightRotate(struct Node *y) {
	struct Node *x = y->left;
	struct Node *T2 = x->right;

	//perform rotation
	x->right = y;
	y->left = T2;

	//update heights
	y->height = max(height(y->left), height(y->right)) + 1;

	x->height = max(height(x->left), height(x->right)) + 1;

	//return new root
	return x;
}

//function to left rotate subtree rooted with x
struct Node* leftRotate(struct Node* x) {
	struct Node *y = x->right;
	struct Node *T2 = y->left;

	//perform rotation
	y->left = x;
	x->right = T2;

	//update heights
	x->height = max(height(x->left), height(x->right)) + 1;

	y->height = max(height(y->left), height(y->right)) + 1;

	//return new root
	return y;
}

//Get a balance factor of Node N
int getBalance(struct Node *N) {
	if(N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

//Recursive function to insert a key in the
//subtree rooted with node and retuens the
//new root of the subtree
struct Node*
insert_tree_node (int doc_id, char *name, int word_count,
		struct Node *root ) {

	//1. perform the normal bst insertion
	/* If the tree is empty, return a new node */
    if (root == NULL) 
        return newNode(doc_id, name, word_count); 
  
    /* Otherwise, recur down the tree */
    if (doc_id < root->doc_id) 
        root->left  = insert_tree_node(doc_id,name,word_count,root->left); 
    else if (doc_id > root->doc_id) 
        root->right = insert_tree_node(doc_id,name,word_count,root->right);
	else  //equal keys not allowed
		return root;    
  
    //2. Update height of this ancestor node
	root->height = 1 + max(height(root->left), height(root->right));

	/*3. Get the balance factor of this ancestor node
		to check whether this node became unbalanced */
	int balance = getBalance(root);

	//If this node becomes unbalanced, then
	//there are 4 cases

	// Left Left Case
	if(balance > 1 && doc_id < (root->left)->doc_id)
		return rightRotate(root);

	//Right Right Case
	if(balance < -1 && doc_id > (root->right)->doc_id)
		return leftRotate(root);

	//Left Right Case
	if(balance > 1 && doc_id > (root->left)->doc_id) {
		root->left = leftRotate(root->right);
		return rightRotate(root);
	}

	//Right Left Case
	if(balance < -1 && doc_id < (root->right)->doc_id) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	//return the unchanged node pointer
	return root;
}

int
bstdb_init ( void ) {
	// This function will run once (and only once) when the database first
	// starts. Use it to allocate any memory you want to use or initialize 
	// some globals if you need to. Function should return 1 if initialization
	// was successful and 0 if something went wrong.

	bst_root = NULL;
	next_id = 0;
	num_comps = 0;
	num_searches = 0;
	return 1;
}

// int create_id() {
// 	int a = MAX / (2^(cnt/2);
// 	int b = 
// }

int
bstdb_add ( char *name, int word_count ) {
	// This function should create a new node in the binary search tree, 
	// populate it with the name and word_count of the arguments and store
	// the result in the tree.
	//
	// This function should also generate and return an identifier that is
	// unique to this document. A user can find the stored data by passing
	// this ID to one of the two search functions below.
	//
	// How you generate this ID is up to you, but it must be an integer. Note
	// that this ID should also form the keys of the nodes in your BST, so
	// try to generate them in a way that will result in a balanced tree.
	//
	// If something goes wrong and the data cannot be stored, this function
	// should return -1. Otherwise it should return the ID of the new node
	//struct Node *tmp;

	// next_id = create_id();

	//if(bst_root == NULL) {
		bst_root = insert_tree_node(next_id, name, word_count, bst_root);
		if(!bst_root)
			return -1;
	//}
	// else {
	// 	tmp = insert_tree_node(next_id, name, word_count, bst_root);
	// 	if(!tmp)
	// 		return -1;
	// }

	return next_id++;
}

int
bstdb_get_word_count ( int doc_id ) {
	// This is a search function. It should traverse the binary search tree
	// and return the word_count of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return -1
	struct Node *p = find_document(doc_id);
	return(p)? p->word_count : -1;
}

char*
bstdb_get_name ( int doc_id ) {
	// This is a search function. It should traverse the binary search tree
	// and return the name of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return NULL or 0
	struct Node *p = find_document(doc_id);
	return(p)? p->name : NULL;
}

int count = 0;
void
len_list(struct Node *ln) {
	if(ln != NULL) {
		len_list(ln->left);
		count++;
		len_list(ln->right);
	}
}

void
bstdb_stat ( void ) {
	// Use this function to show off! It will be called once after the 
	// profiler ends. The profiler checks for execution time and simple errors,
	// but you should use this function to demonstrate your own innovation.
	//
	// Suggestions for things you might want to demonstrate are given below,
	// but in general what you choose to show here is up to you. This function
	// counts for marks so make sure it does something interesting or useful.
	//
	//  + Check if your tree is balanced and print the result
	//
	//  + Does the number of nodes in the tree match the number you expect
	//    based on the number of insertions you performed?
	//
	//  + How many nodes on average did you need to traverse in order to find
	//    a search result? 
	//
	//  + Can you prove that there are no accidental duplicate document IDs
	//    in the tree?
	printf("STAT\n");
    printf("Avg comparisons per search  -> %lf\n",
        (double)num_comps/num_searches);
	
	len_list(bst_root);
    printf("List size matches expected? -> %c\n",
         ((next_id == count)? 'Y' : 'N') );

	printf("Height of the tree  -> %d\n", height(bst_root));

	int tmp = (height(bst_root->left) - height(bst_root->right));
	printf("Balanced Binary Tree  -> %c\n", 
		(tmp>=-1 && tmp<=1) ? 'Y' : 'N' );
}

void
bstdb_quit ( void ) {
	// This function will run once (and only once) when the program ends. Use
	// it to free any memory you allocated in the course of operating the
	// database.
	free_tree_node(bst_root);
}
