#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdbool.h>

// do while 
#define add_node(tree, node, side, str)\
{\
    node->side = create_node(str);\
    tree->size++;\
    node->side->parent = node;\
}\

#define wadd_node(tree, node, side, str)\
{\
    node->side = wcreate_node(str);\
    if(!strcmp( #side, "left"))\
        tree->size++;\
    node->side->parent = node;\
}\

typedef struct Node 
{
    wchar_t* str;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    bool loaded;
} Node;

typedef struct 
{
    Node* root;
    wchar_t* txt;
    int size;
    bool loaded;
} Tree;

    
Tree* tree_init();
Node* create_node(char* str);
Node* wcreate_node(wchar_t* str);
void tree_free(Tree** tree);
void dump_tree_dot(const char* out, Tree* tree);
void save_tree(const char* output, Tree* tree);
Tree* load_tree(const char* input);



#endif /* TREE_H */ 
