#ifndef TREE_H
#define TREE_H

#include <stddef.h>
typedef struct Node 
{
    wchar_t* str;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct 
{
    Node* nodes;
    int size;
} Tree;

    
Tree* tree_init();
Node* create_node(char* str);

#endif /* TREE_H */ 
