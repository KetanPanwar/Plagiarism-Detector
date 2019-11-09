#include<stdio.h>
#include<stdlib.h>
struct tnode
{
  int data;
  struct tnode *left;
  struct tnode *right;
};
void inorder(struct tnode*);
void preorder(struct tnode*);
void postorder(struct tnode*);
void insert(struct tnode**,int);
int count(struct tnode *);
int leafcount(struct tnode *);
int height(struct tnode *);
struct tnode* rinsert(struct tnode *, int);
struct tnode* tdelete(struct tnode*,int );
int search(struct tnode*, int);
int rsearch(struct tnode *,int);
void main()
{
  struct tnode *root;
  int ch,num,k;
   root=NULL;
 while(1)
 {
      printf("\n1.Insert");
      printf("\n2.Preorder");   
      printf("\n3.Postorder");
      printf("\n4.Inorder");
      printf("\n5.No. of nodes");
      printf("\n6.No. of Leaf nodes");
      printf("\n7.Height of a tree");
      printf("\n8.delete a node");
      printf("\n9..Recursively insert..\n");
      printf("\n10..search..\n");
      printf("\n11..search recursively.\n");
      printf("\n12.exit");
      scanf("%d",&ch);
       switch(ch)
       {
         case 1 :printf("Enter the element");
                 scanf("%d",&num);
                 insert(&root,num);
                 break;
        case 2: preorder(root);
                 break; 
        case 3: postorder(root);
                 break;
        case 4:inorder(root);
                 break;
       case 5: k=count(root);
                 printf("\nthe number of nodes=%d",k);
                  break;
       case 6: k=leafcount(root);
                 printf("\nthe number of nodes=%d",k);
                 break;
        case 7: k=height(root);
                 printf("\nthe height of tree=%d",k);

                 break;
        case 8:printf("Enter the node to be deleted\n");
                scanf("%d",&num);
                root=tdelete(root,num);
        case 9 :printf("Enter the element");
                 scanf("%d",&num);
                 root=rinsert(root,num);
                 break;
       case 10:printf("Enter the element");
               scanf("%d",&num);
               k=search(root,num);
               if(k)
                 printf("Element found...\n");
              else
                 printf("Not found..\n");
              break;
       case 11:printf("Enter the element");
               scanf("%d",&num);
               k=rsearch(root,num);
               if(k)
                 printf("Element found...\n");
              else
                 printf("Not found..\n");
              break;
       case 12:exit(0);
       }
    }
  }    
  int rsearch(struct tnode *t,int key)
  {
    int k;
    if(t!=NULL)
    {
      if(t->data==key)
        return 1;
      if(key <t->data)
       k=rsearch(t->left,key);
      else
       k=rsearch(t->right,key);
     return k;
     }
    return 0;
  }
int search(struct tnode *t, int key)
  {
    struct tnode *curr;

    curr=t;
    while((curr!=NULL)&&(curr->data!=key))
    {
      if(key<curr->data)
        curr=curr->left;
      else
       curr=curr->right;
    }
    if(curr==NULL)
     return 0;
    return 1;
  }
  struct tnode* rinsert(struct tnode* t, int x)
  {
    struct tnode *temp;
    //empty tree, first element inserted
    if(t==NULL)
    {
      temp=(struct tnode*)malloc(sizeof(struct tnode));
      temp->data=x;
      temp->left=temp->right=NULL;
      return temp;
    }     
   if(x<t->data)
     //insert into the left subtree
     //and return the root of the left subtree
     t->left=rinsert(t->left,x);
   else
      //insert into the left subtree
      //and return the root of the left subtree
      t->right=rinsert(t->right,x);
   return t;
  }
struct tnode* tdelete(struct tnode *t, int key)
  {
    struct tnode *temp,*curr, *prev,*p,*q;

    curr=t;
    prev=NULL;

    while((curr!=NULL)&&(curr->data!=key))
    {
      prev=curr;
      if(key<curr->data)
         curr=curr->left;
      else
         curr=curr->right;
    }
    if(curr==NULL)
    {
      printf("Node not found..\n");
      return t;
    }
    //deleting a node with one or no subtrees
    if((curr->left==NULL)||(curr->right==NULL))
    {
       if(curr->left==NULL)//if no left subtree
         q=curr->right;//get the right subtree
      else
         q=curr->left;//no right subtree, get the left subtree
    
     if(prev==NULL)//deleting root with one subtree
       return q;
     if(curr==prev->left)
      prev->left=q;
     else
      prev->right=q;
    free(curr);
   }
   else
   {
    //deleting a node with two subtrees
    //find the inorder successor
    p=NULL;//used for parent of inorder successor
    temp=curr->right;
    while(temp->left!=NULL)
    {
      p=temp;
      temp=temp->left;
    }
   //inorder successor is found..(temp)    
    if(p!=NULL)
     p->left=temp->right;
    else
     curr->right=temp->right;
   //replace the node's(to be deleted) data with the data of inorder successor
     curr->data=temp->data;
   //delete the inorder successor
     free(temp);
  }
  return t;
 }
  int height(struct tnode *t)
  {
    if(t==NULL)
      return -1;
   if((t->left==NULL)&&(t->right==NULL))
       return 0;
    int l=height(t->left);
    int r=height(t->right);
    if(l>r)
      return(l+1);
    return(r+1);
  }
int leafcount(struct tnode *t)
 {
    if(t==NULL)
      return 0;
    if((t->left==NULL)&&(t->right==NULL))
       return 1;
    int l=leafcount(t->left);//find no. of leaf nodes in left subtree
    int r=leafcount(t->right);//find no of leaf nodes in right subtee
    return(l+r);
  } 
 int count(struct tnode *t)
 {
    if(t==NULL)
      return 0;
    int l=count(t->left);//find no. of nodes in left subtree
    int r=count(t->right);//find no of nodes in right subtee
    return(l+r+1);
  }
 void insert(struct tnode **t, int x)
 {
   struct tnode *temp,*prev,*curr;
   //create node

   temp=(struct tnode*)malloc(sizeof(struct tnode));
   temp->data=x;
   temp->left=temp->right=NULL;

   //if empty tree
   if(*t==NULL)
     *t=temp;//make root point to the first element
   else
   {
     prev=NULL;
     curr=*t;
     while(curr!=NULL)
     {
       prev=curr;
       if(x<curr->data)
         curr=curr->left;//move left
       else
          curr=curr->right;//move right
     }
     if(x<prev->data)
       prev->left=temp;
     else
       prev->right=temp;
    }
  }   
void preorder(struct tnode*t)
   {
     if(t!=NULL)
      {
         printf("%d   ",t->data);
         preorder(t->left);
         preorder(t->right);
      }
    }
   void postorder(struct tnode*t)
   {
     if(t!=NULL)
      {
         postorder(t->left);
         postorder(t->right);
         printf("%d   ",t->data);
        
      }
    }
   void inorder(struct tnode*t)
   {
     if(t!=NULL)
      {
         inorder(t->left);
         printf("%d   ",t->data);
         inorder(t->right);
      }
    }