#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
   
struct Node 
{ 
    char key; 
    struct Node *left;
    struct Node *right; 
}; 
   
// A utility function to create a new BST node 
struct Node *newNode(char item) 
{ 
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node)); 
    temp->key = item; 
    temp->left = temp->right = NULL; 
    return temp; 
} 
   
// A utility function to do inorder traversal of BST 
void inorder(struct Node *root) 
{ 
    if (root != NULL) 
    { 
        inorder(root->left); 
        printf("%c \n", root->key); 
        inorder(root->right); 
    } 
} 
   
/* A utility function to insert a new node with given key in BST */
struct Node* insert(struct Node* node, char key) 
{ 
    /* If the tree is empty, return a new node */
    if (node == NULL) 
        return newNode(key); 
  
    /* Otherwise, recur down the tree */
    if (key <= node->key) 
        node->left  = insert(node->left, key); 
    else if (key > node->key) 
        node->right = insert(node->right, key);    
  
    /* return the (unchanged) node pointer */
    return node; 
} 

//function to search a given key
struct Node* search_item(struct Node *root, char key) {
    //root is null
    if(root == NULL)
        return root;

    if(root->key == key) {
        printf("\n%c in Tree\n", key);
        return root;
    }

    else if(root->key < key)
        return search_item(root->right, key);

    else 
        return search_item(root->left, key);
}

char random_gen() {
    char alphabet[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
                          'H', 'I', 'J', 'K', 'L', 'M', 'N',  
                          'O', 'P', 'Q', 'R', 'S', 'T', 'U', 
                   
                          'V', 'W', 'X', 'Y', 'Z' };
    // srand(time(0));
    return alphabet[rand() % 26]; 
}

void searchCall(struct Node *root) {
    char item;
    struct Node *temp = NULL; 
    
        item = random_gen();
        temp = search_item(root, item);
        if(temp == NULL)
            printf("\n%c not in Tree\n", item);
        free(temp);
}

void deleteTree(struct Node* root) {
    if(root == NULL)
        return;
    
    //first delete both subtrees
    deleteTree(root->left);
    deleteTree(root->right);

    //then delete the node
    printf("\ndeleting node %c", root->key);
    free(root);
}
  
int main() 
{ 
    struct Node *root = NULL; 

    char data[] = {"FLOCCINAUCINIHILIPILIFICATTION"};
    root = insert(root, data[0]);
    for(int i=1;data[i]!='\0';i++) {
        insert(root, data[i]);
        // printf("\t%c", data[i]);
    }

    inorder(root); 

    searchCall(root);
    searchCall(root);
   
    deleteTree(root);
    root = NULL;

    return 0; 
} 
