#include "tree.h"
#include <stdlib.h>
#include <wchar.h>

Tree* tree_init()
{
    Tree* res = (Tree*)calloc(1, sizeof(Tree)); 

    res->nodes = (Node*)calloc(1, sizeof(Node));
    res->size = 1;

    res->nodes->str = L"Неизвестно кто?";
    res->nodes->left = res->nodes->right = res->nodes->parent = NULL;

    return res;
}

Node* create_node(char* str)
{
    static wchar_t buff[256] = {};
    mbstowcs(buff, str, 255);
    //memccpy(buff, src, '\0', 256);
    
    
    Node* res = (Node*)calloc(1, sizeof(Node));
    res->parent = res->left = res->right = NULL;
    res->str = wcsdup(buff);
    return res;
}

void node_free(Node* node)
{
    if(!(node->left))
        node_free(node->left);
    if(!(node->right))
        node_free(node->right);

    free(node->str);
    free(node);
    return;
}

void tree_free(Tree* tree)
{
    node_free(tree->nodes->left);
    node_free(tree->nodes->right);
    node_free(tree->nodes);
    free(tree);
}
