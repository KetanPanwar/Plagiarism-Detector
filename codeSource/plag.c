void readData(FILE *fp, int offset, node_t *st)
{
    fseek(fp, offset, SEEK_SET);
    fread(st, sizeof(node_t), 1, fp);
}

FILE *init_tree(tree_t *ptr_tree, const char *name)
{
    freopen("1136_op.txt", "w", stdout);    //redirects output to the output file mentioned
    FILE *fp = fopen(name, "r+");
    if (fp == NULL)     //if tree.dat file is empty, it should be created
    {
        fp = fopen(name, "w");
        ptr_tree->free_head = -1;
        ptr_tree->root = -1;
        fwrite(ptr_tree, sizeof(tree_t), 1, fp);
        fclose(fp);
        fp = fopen(name, "r+");
    }
    fread(ptr_tree, sizeof(tree_t), 1, fp);
    return fp;
}

void close_tree(tree_t *ptr_tree, FILE *fp)
{
    fclose(fp);
}

void inorder(int offset, FILE *fp)
{
    if (offset != -1)
    {
        //getting the required node from file
        node_t temp;
        readData(fp, offset, &temp);

        //actual tree traversal part
        inorder(temp.left_offset, fp);
        printf("%d ", temp.key);
        inorder(temp.right_offset, fp);
    }
}

void display_inorder(const tree_t *ptr_tree, FILE *fp)
{
    inorder(ptr_tree->root, fp);
    printf("\n");
}

void preorder(int offset, FILE *fp)
{
    if (offset != -1)
    {
        //getting the required node from file
        node_t temp;
        readData(fp, offset, &temp);

        //actual tree traversal part
        printf("%d ", temp.key);
        preorder(temp.left_offset, fp);
        preorder(temp.right_offset, fp);
    }
}

void display_preorder(const tree_t *ptr_tree, FILE *fp)
{
    preorder(ptr_tree->root, fp);
    printf("\n");
}

void insert_key(tree_t *ptr_tree, int key, FILE *fp)
{

    node_t temp_node;
    int temp;       //offset where the new node should be inserted

    int wasFreeHeadUsed = 0;    //used in case of a duplicate key
    if (ptr_tree->free_head == -1)      //if free list is empty then just go the end to insert
    {
        fseek(fp, 0, SEEK_END);
        temp = ftell(fp);
    }
    else        //otherwise take the first free node
    {
        temp = ptr_tree->free_head;
        readData(fp, temp, &temp_node);
        ptr_tree->free_head = temp_node.right_offset;
        wasFreeHeadUsed = 1;
    }

    temp_node.key = key;
    temp_node.left_offset = temp_node.right_offset = -1;

    if (ptr_tree->root == -1)       //if tree is empty
    {
        ptr_tree->root = temp;
        fseek(fp, 0, SEEK_SET);
        fwrite(ptr_tree, sizeof(tree_t), 1, fp);
        fseek(fp, temp, SEEK_SET);
        fwrite(&temp_node, sizeof(node_t), 1, fp);
    }
    else        //if tree is not empty
    {
        int pres = ptr_tree->root;
        int prev = -1;

        while (pres != -1)  //find the position where we should insert the node
        {
            prev = pres;

            node_t presNode;
            readData(fp, pres, &presNode);

            if (key < presNode.key) //predicate < comes into picture
            {
                pres = presNode.left_offset;
            }
            else if(key > presNode.key)
            {
                pres = presNode.right_offset;
            }
            else    //ignore same keys
            {
                break;
            }
            
        }

        node_t prevNode;
        readData(fp, prev, &prevNode);
        if (key < prevNode.key)
        {
            prevNode.left_offset = temp;
        }
        else if(key > prevNode.key)
        {
            prevNode.right_offset = temp;
        }
        else        //ignore the duplicate key, reset back ptr_tree and return
        {
            if(wasFreeHeadUsed)
            {
                temp_node.left_offset = -1;
                temp_node.right_offset = ptr_tree->free_head;
                ptr_tree->free_head = temp;
            }
            return;
        }
        

        //write ptr_tree, temp_node, and prev
        fseek(fp, 0, SEEK_SET);
        fwrite(ptr_tree, sizeof(tree_t), 1, fp);

        fseek(fp, prev, SEEK_SET);
        fwrite(&prevNode, sizeof(node_t), 1, fp);

        fseek(fp, temp, SEEK_SET);
        fwrite(&temp_node, sizeof(node_t), 1, fp);
    }
}

void delete_key(tree_t *ptr_tree, int key, FILE *fp)
{
    if (ptr_tree->root == -1)
    {
        printf("-1\n");
        return;
    }

    //get to the node to be deleted
    int temp = ptr_tree->root;
    int prev = -1;

    node_t tempNode, prevNode;

    readData(fp, temp, &tempNode);
    while (temp != -1 && tempNode.key != key)
    {
        prev = temp;
        prevNode = tempNode;
        if (key < tempNode.key) //predicate < comes into picture
        {
            temp = tempNode.left_offset;
        }
        else
        {
            temp = tempNode.right_offset;
        }

        readData(fp, temp, &tempNode);
    }

    if (temp != -1) //if key is found
    {
        if (prev != -1)     //we're not dealing with the root
        {
            if (tempNode.right_offset == -1 && tempNode.left_offset == -1) //if node to be deleted is leaf
            {
                if (temp == prevNode.left_offset)
                {
                    prevNode.left_offset = -1;
                }
                else
                {
                    prevNode.right_offset = -1;
                }
            }
            else if (tempNode.right_offset == -1 || tempNode.left_offset == -1) //if node to be deleted has only one child
            {
                int toBePlaced = (tempNode.left_offset == -1) ? tempNode.right_offset : tempNode.left_offset;
                if (temp == prevNode.left_offset)
                {
                    prevNode.left_offset = toBePlaced;
                }
                else
                {
                    prevNode.right_offset = toBePlaced;
                }
            }
            else //if node to be deleted has two children
            {
                //find the inorder successor
                int insuc = tempNode.right_offset;
                int insucPred = -1;

                node_t insucNode, insucPredNode;
                readData(fp, insuc, &insucNode);
                while (insucNode.left_offset != -1)
                {
                    insucPred = insuc;
                    insucPredNode = insucNode;
                    insuc = insucNode.left_offset;
                    readData(fp, insuc, &insucNode);
                }

                if (insucPred != -1)
                {
                    insucPredNode.left_offset = insucNode.right_offset;
                    insucNode.right_offset = tempNode.right_offset;
                }

                insucNode.left_offset = tempNode.left_offset;

                if (temp == prevNode.left_offset)
                {
                    prevNode.left_offset = insuc;
                }
                else
                {
                    prevNode.right_offset = insuc;
                }

                //write back insucNode and insucPredNode
                fseek(fp, insuc, SEEK_SET);
                fwrite(&insucNode, sizeof(node_t), 1, fp);

                if (insucPred != -1)
                {
                    fseek(fp, insucPred, SEEK_SET);
                    fwrite(&insucPredNode, sizeof(node_t), 1, fp);
                }
            }
        }
        else //root is to be deleted
        {
            if (tempNode.right_offset == -1 && tempNode.left_offset == -1)
            {
                ptr_tree->root = -1;
            }
            else if (tempNode.right_offset == -1 || tempNode.left_offset == -1)
            {
                ptr_tree->root = (tempNode.left_offset == -1) ? tempNode.right_offset : tempNode.left_offset;
            }
            else
            {
                int insuc = tempNode.right_offset;
                int insucPred = -1;

                node_t insucNode, insucPredNode;

                readData(fp, insuc, &insucNode);
                while (insucNode.left_offset != -1)
                {
                    insucPred = insuc;
                    insucPredNode = insucNode;

                    insuc = insucNode.left_offset;
                    readData(fp, insuc, &insucNode);
                }

                if (insucPred != -1)
                {
                    insucPredNode.left_offset = insucNode.right_offset;
                    insucNode.right_offset = tempNode.right_offset;
                }

                insucNode.left_offset = tempNode.left_offset;

                ptr_tree->root = insuc;

                //write back insucNode and insucPredNode
                fseek(fp, insuc, SEEK_SET);
                fwrite(&insucNode, sizeof(node_t), 1, fp);

                if (insucPred != -1)
                {
                    fseek(fp, insucPred, SEEK_SET);
                    fwrite(&insucPredNode, sizeof(node_t), 1, fp);
                }
            }
        }

        //write back ptr_tree, and prevNode (if present), temp

        //free up temp by adding it to free_head thing
        tempNode.left_offset = -1;
        tempNode.right_offset = ptr_tree->free_head;
        ptr_tree->free_head = temp;

        fseek(fp, 0, SEEK_SET);
        fwrite(ptr_tree, sizeof(tree_t), 1, fp);

        if (prev != -1)
        {
            fseek(fp, prev, SEEK_SET);
            fwrite(&prevNode, sizeof(node_t), 1, fp);
        }

        fseek(fp, temp, SEEK_SET);
        fwrite(&tempNode, sizeof(node_t), 1, fp);
    }
    else
    {
        printf("-1\n");
        return;
    }
    
}
