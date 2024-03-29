#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdbool.h>
#include "../id_table/id_table.h"

typedef enum
{
    FUNCTION = 0,
    VARIABLE,
    VARIABLE_DEF,
    CONSTANT,
    OPERATOR,
    CONDITION,
    FUNC_CALL,
    SEMICOLON,
    BLOCK_OF_STATEMENTS,
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
    node_type_t type; 
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct Id_node
{
    struct Node node_base;
    value_t value;
    bool alloc_name;
} Id_node;

typedef struct Func_node
{
    struct Id_node id_base;
    id_table_t* id_table;
    int num_of_args;
} Func_node;

typedef struct Blk_node
{
    struct Node node_base;
    id_table_t* id_table;
} Blk_node;

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

void* create_node(node_type_t type, value_t value);

Node* copy_node(const Node* node);
Node* copy_subtree(const Node* subtree);

void dump_tree_dot(const char* output, Tree* tree);
void dump_tree_tex(const char* output, Tree* tree);

#endif /* TREE_H */ 
