#ifndef TREE_H
#define TREE_H

typedef struct Node 
{
    char* str;
    struct Node* left;
    struct Node* right;

} Node;

typedef struct 
{
    Node* nodes;
    int size;
} Tree;

#endif /* TREE_H */ 
