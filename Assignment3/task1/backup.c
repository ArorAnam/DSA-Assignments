#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
struct tree
{
char data;
struct tree *left;
struct tree *right;
};
struct tree* insert(struct tree *root,char data)
{

struct tree *par;
struct tree *n=malloc(sizeof(struct tree));
n->left=NULL;
n->data=data;
n->right=NULL;

if(root==NULL)
{

root=n;

}
else
{
par=root;
while(par!=NULL)
{
if(par->data>data)
{
if(par->left==NULL)
{
par->left=n;
break;
}

par=par->left;

}
else if(par->data<data || par->data==data)
{
if(par->right==NULL)
{
par->right=n;
break;
}
par=par->right;
}
}
}


return(root);
}
struct tree* find(struct tree *root,char data)
{
if(root==NULL)
{

return(NULL);
}

if(data<root->data)
return(find(root->left,data));
else if(data>root->data)
return(find(root->right,data));
return(root);
}
void inorder(struct tree *root)
{
if(root)
{
inorder(root->left);
printf("%c ",root->data);
inorder(root->right);
}
}
void del(struct tree *root)
{
if(root==NULL)
return;
del(root->left);
del(root->right);
free(root);

}
int menu()
{
int choice;
printf("\n1. Print string in sorted order");
printf("\n2. Search tree");
printf("\n3. Delete Tree");
printf("\n4. Exit");
printf("\nEnter Your Choice: ");
scanf("%d",&choice);
return(choice);
}
int main()
{
char a,arr[31]={"FLOCCINAUCINIHILIPILIFICATTION"};
struct tree *root=NULL;

struct tree *x;
int i;

for(i=0;i<30;i++)
{

root=insert(root,arr[i]);
}



while(1)
{
system("cls");
switch(menu())
{
case 1:
if(root==NULL)
{
printf("Tree Doesn't Exist");
}
else
inorder(root);
break;
case 2:
printf("Enter Element to find: ");
scanf("%s",&a);
x=find(root,a);
if(x!=NULL)
printf("%c in Tree\n",a);
else
printf("%c not in Tree\n",a);
break;
case 3:
del(root);
root=NULL;
printf("Deletion Successful");
break;
case 4:
exit(0);
default:
printf("Invalid Choice");
}
getch();
}
}