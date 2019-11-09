#include <stdio.h>
#include <stdlib.h>
#include "a4.h"
FILE *fpout;
FILE* init_tree(tree_t* mytree, const char* name){
    fpout = fopen("0202_op.txt", "a");
    FILE* fp = fopen(name, "r+");
    if(fp == NULL)
    {
        fp = fopen(name, "w");
        mytree->free_head = -1;
        mytree->root = -1;
        fwrite(mytree, sizeof(tree_t), 1, fp);
        fclose(fp);
        fp = fopen(name, "r+");
    }
    fread(mytree, sizeof(tree_t), 1, fp);
    return fp;
}
void close_tree(tree_t *mytree, FILE *fp){
        fclose(fp);
}
void insert_key(tree_t *ptr_tree, int key, FILE *fp){
    node_t temp_node;
    int temp;   //location of the key to be inserted
    int f=0;
    // if no free node present add to end of the file
    if(ptr_tree->free_head == -1)
    {
        fseek(fp, 0, SEEK_END); 
        temp = ftell(fp);
        //printf("off1 : %d\n", temp);
        
    }
    // if free node present get the offset of the freenode and update the free list
    else
    {
        f=1;
        temp = ptr_tree->free_head;
        fseek(fp, temp, SEEK_SET);
        //printf("off2 : %d\n", temp);
        fread(&temp_node, sizeof(node_t), 1, fp);
        ptr_tree->free_head = temp_node.left_offset;
    }
    
    temp_node.key = key;
    temp_node.left_offset = -1;
    temp_node.right_offset = -1;
    //If the tree is empty add temp_node as first element and update the root
    if(ptr_tree->root == -1)
    {
        ptr_tree->root = temp;
        temp_node.left_offset = -1;
        temp_node.right_offset = -1;
        fseek(fp, 0, SEEK_SET); fwrite(ptr_tree, sizeof(tree_t), 1, fp);
        fseek(fp, temp, SEEK_SET); fwrite(&temp_node, sizeof(node_t), 1, fp);
    }
    else{
        int prev = -1; int pres = ptr_tree->root;
        node_t pres_node; node_t prev_node;
        fseek(fp, pres, SEEK_SET);
        fread(&pres_node, sizeof(node_t), 1, fp);
        //Find correct position for insertion
        while(pres != -1 && pres_node.key != key)
        {   
            prev = pres;
            prev_node = pres_node;
            //printf("%d ",pres_node.key);
            if(pres_node.key < key){
                    pres = pres_node.right_offset;
            }
            else{
                    pres = pres_node.left_offset;
            }
            if(pres != -1)
            {   
                fseek(fp, pres, SEEK_SET);
                fread(&pres_node, sizeof(node_t), 1, fp);
            }
        }
        //The value(key) is not a duplicate then insert at offset temp
        if(pres == -1){
                if(prev_node.key < key){
                    prev_node.right_offset = temp;
                }
                else{
                    prev_node.left_offset = temp;
                }
                fseek(fp, temp, SEEK_SET); fwrite(&temp_node, sizeof(node_t), 1, fp);
                fseek(fp, prev, SEEK_SET); fwrite(&prev_node, sizeof(node_t), 1, fp);
                fseek(fp, 0, SEEK_SET); fwrite(ptr_tree, sizeof(tree_t), 1, fp);
        }
        // The value(key) of is a duplicate
        else{
                //if the free head was changed reset it back.
                if(f==1){
                   ptr_tree->free_head = temp;
                   fseek(fp, 0, SEEK_SET); fwrite(ptr_tree, sizeof(tree_t), 1, fp);
               }
        }
        
    }
}
void bst_inorder(int elem,FILE *fp){
    if(elem == -1){
        return ;
    }
    node_t temp;
    // left child,parent,right child
    fseek(fp, elem, SEEK_SET);
    fread(&temp, sizeof(node_t), 1, fp);
    bst_inorder(temp.left_offset,fp);
    printf("%d ",temp.key);
    fprintf(fpout,"%d ",temp.key);
    bst_inorder(temp.right_offset,fp);
}
void display_inorder(const tree_t *ptr_tree, FILE *fp){
    if(ptr_tree->root != -1){
            bst_inorder(ptr_tree->root,fp);
        }
        printf("\n");
        fprintf(fpout,"\n");
    
}
void bst_preorder(int elem,FILE *fp){
    if(elem == -1){
        return ;
    }
    node_t temp;
    //parent,left child,right child
    fseek(fp, elem, SEEK_SET);
    fread(&temp, sizeof(node_t), 1, fp);
    printf("%d ",temp.key);
    fprintf(fpout,"%d ",temp.key);
    bst_preorder(temp.left_offset,fp);
    bst_preorder(temp.right_offset,fp);
}
void display_preorder(const tree_t *ptr_tree, FILE *fp){
        if(ptr_tree->root != -1){
            bst_preorder(ptr_tree->root,fp);
        }
        printf("\n");
        fprintf(fpout,"\n");
}
void delete_key(tree_t *ptr_tree, int key, FILE *fp){
    int prev = -1; int pres = ptr_tree->root;
    int q;
    //Empty tree
    if(pres == -1){
        printf("-1\n");
        return;
    }
    node_t pres_node; node_t prev_node;
    fseek(fp, pres, SEEK_SET);
    fread(&pres_node, sizeof(node_t), 1, fp);
    //Search and find the node
    while(pres != -1 && pres_node.key != key){
        prev = pres;
        prev_node = pres_node;
        //printf("%d",pres_node.key);
        if(pres_node.key < key){
            pres = pres_node.right_offset;
        }
        else{
            pres = pres_node.left_offset;
        }
        if(pres != -1)
        {   
            fseek(fp, pres, SEEK_SET);
            fread(&pres_node, sizeof(node_t), 1, fp);
        }
    }
    //node not present in the tree
    if(pres == -1){
        printf("-1\n");
        return;
    }
    //If node has one only one subtree or no subtree(i.e leaf)
    if(pres_node.left_offset == -1 || pres_node.right_offset == -1){
            if(pres_node.left_offset == -1){
                q=pres_node.right_offset;
            }
            else{
                q=pres_node.left_offset;
            }
            //root with one subtree
            if(prev == -1){
                pres_node.left_offset = ptr_tree->free_head;
                //pres_node.right_offset = -1;
                ptr_tree->free_head = pres;
                ptr_tree->root = q;
                fseek(fp, 0, SEEK_SET); fwrite(ptr_tree, sizeof(tree_t), 1, fp);
                fseek(fp, pres, SEEK_SET); fwrite(&pres_node, sizeof(node_t), 1, fp);
                return;
            }
            if(prev_node.left_offset == pres){
                    prev_node.left_offset = q;
            }
            else{
                    prev_node.right_offset = q;
            }
            pres_node.left_offset = ptr_tree->free_head;
            //pres_node.right_offset = -1;
            ptr_tree->free_head = pres;
            fseek(fp, 0, SEEK_SET); fwrite(ptr_tree, sizeof(tree_t), 1, fp);
            fseek(fp, pres, SEEK_SET); fwrite(&pres_node, sizeof(node_t), 1, fp);
            fseek(fp, prev, SEEK_SET); fwrite(&prev_node, sizeof(node_t), 1, fp);
    }
    // If both the subtrees are present
    else{
            int parent = -1;
            int temp = pres_node.right_offset;
            node_t temp_node;
            node_t parent_node;
            fseek(fp, temp, SEEK_SET);
            fread(&temp_node, sizeof(node_t), 1, fp);
            //find the inorder successor
            while(temp_node.left_offset != -1){
                parent = temp;
                parent_node = temp_node;
                temp = temp_node.left_offset;
                fseek(fp, temp, SEEK_SET);
                fread(&temp_node, sizeof(node_t), 1, fp);
            }
            //Inorder successor is found i.e temp
            if(parent != -1){
                parent_node.left_offset = temp_node.right_offset;
                fseek(fp, parent, SEEK_SET); fwrite(&parent_node, sizeof(node_t), 1, fp);
                
            }
            //Element to right of pres itself is successor , so pres becomes the parent basically 
            else{
                pres_node.right_offset = temp_node.right_offset;
            }
            //replace node to be removed data with data of the inorder successor
            pres_node.key = temp_node.key;
            // add temp to freelist
            temp_node.left_offset = ptr_tree->free_head;
            ptr_tree->free_head = temp;
            fseek(fp, 0, SEEK_SET); fwrite(ptr_tree, sizeof(tree_t), 1, fp);
            fseek(fp, temp, SEEK_SET); fwrite(&temp_node, sizeof(node_t), 1, fp);
            fseek(fp, pres, SEEK_SET); fwrite(&pres_node, sizeof(node_t), 1, fp);

    }
}
