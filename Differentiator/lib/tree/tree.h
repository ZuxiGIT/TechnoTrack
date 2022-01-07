#ifndef TREE_H
#define TREE_H

// Try static library for tree

#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    FUNC = 0,
    VAR,
    CONST,
    OPER,
    EMPTY,
    UNDEFINED
} type_t;

typedef union // requires details
{
    double num;
    char* text;
} value_t;  

typedef struct Node 
{
    value_t value;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    type_t type; 
    bool alloc;
} Node;

typedef struct 
{
    Node* root;
    int size;
    char* text;
    bool loaded;
} Tree;

    
Tree* tree_init();
void tree_free(Tree** tree);

void dump_tree_dot(const char* out, Tree* tree);
void save_tree(const char* output, Tree* tree);
Tree* load_tree(const char* input);

Tree* parse_tree_from_source(const char* input);

Node* create_node(type_t type, value_t value);

Node* copy_node(const Node* node);
Node* copy_subtree(const Node* subtree);

void dump_tree_dot(const char* output, Tree* tree);
void dump_tree_tex(const char* output, Tree* tree);

#endif /* TREE_H */ 
