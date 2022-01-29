#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdbool.h>
#include "../id_table/id_table.h"

typedef enum
{
    FUNCTION = 0,
    VARIABLE,
    CONSTANT,
    OPERATOR,
    CONDITION,
    SEMICOLON,
    EMPTY,
    UNDEFINED
} node_type_t;

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
    node_type_t type; 
    bool alloc;
    id_table_t* id_table;
} Node;

typedef struct 
{
    unsigned long long hash;
    Node* root;
    int size;
    char* text;
    bool loaded;
} Tree;

    
unsigned long long hash(const void* _str, int len);
unsigned long long subtree_hash(const Node* node);
unsigned long long tree_hash(const Tree* tree);

Tree* tree_init();
void tree_free(Tree** tree);
void node_free(Node* node);

void save_tree(const char* output, Tree* tree);
Tree* load_tree(const char* input);

Tree* parse_tree_from_source(const char* input);

Node* create_node(node_type_t type, value_t value);

Node* copy_node(const Node* node);
Node* copy_subtree(const Node* subtree);

void dump_tree_dot(const char* output, Tree* tree);
void dump_tree_tex(const char* output, Tree* tree);

#endif /* TREE_H */ 
