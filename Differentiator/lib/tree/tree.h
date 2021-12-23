#ifndef TREE_H
#define TREE_H

#include <stddef.h>

typedef enum
{
    FUNC = 0,
    VAR,
    CONST,
    OPER,
    UNDEFINED
} type_t;

typedef union
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
} Node;

typedef struct 
{
    Node* root;
    int size;
    char* text;
} Tree;

    
Tree* tree_init();
void tree_free(Tree** tree);

void dump_tree_dot(const char* out, Tree* tree);
void save_tree(const char* output, Tree* tree);
Tree* load_tree(const char* input);

Tree* parse_tree_from_source(const char* input);

Node* create_node(type_t type, value_t value);
Node* create_empty_node();
Node* _create_node( type_t type, value_t value,
                    Node* parent, Node* left, Node* right);

#define add_node(tree, node, side, type, value)\
{\
    node->side = _create_node(type, (value_t)value, node, NULL, NULL);\
    tree->size++;\
}

#endif /* TREE_H */ 
