/* This function traverse the skewed binary tree and 
   stores its nodes pointers in vector nodes[] */
void storeBSTNodes(struct Node* root, struct Node *node) 
{ 
    // Base case 
    if (root==NULL) 
        return; 
  
    // Store nodes in Inorder (which is sorted 
    // order for BST) 
    storeBSTNodes(root->left, node->left); 
    node = root; 
    storeBSTNodes(root->right, node->right); 
} 
  
/* Recursive function to construct binary tree */
Node* buildTreeUtil(vector<Node*> &nodes, int start, 
                   int end) 
{ 
    // base case 
    if (start > end) 
        return NULL; 
  
    /* Get the middle element and make it root */
    int mid = (start + end)/2; 
    Node *root = nodes[mid]; 
  
    /* Using index in Inorder traversal, construct 
       left and right subtress */
    root->left  = buildTreeUtil(nodes, start, mid-1); 
    root->right = buildTreeUtil(nodes, mid+1, end); 
  
    return root; 
} 
  
// This functions converts an unbalanced BST to 
// a balanced BST 
Node* buildTree(struct Node* root) 
{ 
    // Store nodes of given BST in sorted order 
    struct Node *store_root; 
    storeBSTNodes(root, store_root); 
  
    // Constucts BST from nodes[]  
    return buildTreeUtil(nodes, 0, n-1); 
} 