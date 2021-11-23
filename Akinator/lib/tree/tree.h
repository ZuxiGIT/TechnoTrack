#ifndef TREE_H
#define TREE_H

#include <stddef.h>

#define add_node(tree, node, side, str)\
{\
    node->side = create_node(str);\
    tree->size++;\
    node->side->parent = node;\
}\

typedef struct Node 
{
    wchar_t* str;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct 
{
    Node* root;
    int size;
} Tree;

    
Tree* tree_init();
Node* create_node(char* str);
void tree_free(Tree* tree);
void dump_tree_dot(const char* out, Tree* tree);

#endif /* TREE_H */ 
