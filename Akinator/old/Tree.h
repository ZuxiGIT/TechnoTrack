#ifndef TREE_H
#define TREE_H
#include <stddef.h>

typedef struct Node
{
    char* data;
    struct Node* left;
    struct Node* right;
} Node;

#define NODE(str) createNode(str, NULL, NULL)

Node* readTreeFromFile(const char* path);
Node* createNode(char* str, Node* lft, Node* rght);
void deleteTree(Node* root);
void printTree(const Node* node);



#endif /* TREE_H */