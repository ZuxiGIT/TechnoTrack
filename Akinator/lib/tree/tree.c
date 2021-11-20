#include "tree.h"
#include <stdlib.h>
#include <wchar.h>
#include <assert.h>
#include <stdio.h>

Tree* tree_init()
{
    Tree* res = (Tree*)calloc(1, sizeof(Tree)); 

    res->root = (Node*)calloc(1, sizeof(Node));
    res->size = 1;

    res->root->str = L"Неизвестно кто?";
    res->root->left = res->root->right = res->root->parent = NULL;

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
    assert(node);
    if((node->left))
        node_free(node->left);
    if((node->right))
        node_free(node->right);

    free(node->str);
    free(node);
    return;
}

void tree_free(Tree* tree)
{
    node_free(tree->root->left);
    node_free(tree->root->right);
    free(tree->root);
    free(tree);
}

static int _shift(char* buff, int shift)
{
    int ret = shift;
    for(; shift > 0; shift--)
        sprintf(buff, "\t");

    return ret;
}

#define _SHIFT buff_pos += _shift(dump_buff + buff_pos, shift);
#define dump_node_dot(node) _dump_node_dot(node, dump_buff, buff_pos, shift);
#define curr_pos dump_buff + buff_pos

int _dump_node_dot(Node* node, char* dump_buff, int buff_pos, int shift)
{
    int ret = buff_pos;

    buff_pos += sprintf(curr_pos, "Node_%c[shape=record, label=\"{ ", 
                        (char)((long)node & 0xff));

    buff_pos += wcstombs(curr_pos, node->str, 4096 - ret);
    
    buff_pos += sprintf(curr_pos, " | \"<left> yes | <right> no\"}\"];");

    _SHIFT;

    if(node->left)
    { 
        buff_pos += sprintf(curr_pos, "Node_%c:<left>->Node_%c;\n",
                            (char)((long)node & 0xff),
                            (char)((long)node->left & 0xff));
        buff_pos += dump_node_dot(node->left);
    }

    if(node->right)
    {
        buff_pos += sprintf(curr_pos, "Node_%c:<right>->Node_%c;\n",
                            (char)((long)node & 0xff),
                            (char)((long)node->right & 0xff));
        buff_pos += dump_node_dot(node->right);
    }

}


void dump_tree_dot(char* output, Tree* tree)
{
    assert(output && "NOT Valid output file");

    static char dump_buff[4096] = {};
    static int buff_pos = 0;
    static int shift = 0;

    buff_pos += sprintf(curr_pos , "digraph tree{\n\trankdir=HR;\n");
    shift++;
    
    _SHIFT;
    
    buff_pos += dump_node_dot(tree->root);
}
