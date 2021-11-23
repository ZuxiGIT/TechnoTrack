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
    static wchar_t buff[64] = {};
    mbstowcs(buff, str, 63);
    //memccpy(buff, src, '\0', 256);
    
    
    Node* res = (Node*)calloc(1, sizeof(Node));

    res->parent = res->left = res->right = NULL;
    res->str = wcsdup(buff);

    wmemset(buff, L'\0', 64);
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

    _SHIFT;

    buff_pos += sprintf(curr_pos, "Node_%hhu[shape=record, label=\"{ \"", 
                        (unsigned char)((long)node & 0xff));

    buff_pos += wcstombs(curr_pos, node->str, 4096 - ret);
    
    buff_pos += sprintf(curr_pos, "\" | {<left> yes | <right> no}}\"];\n");


    if(node->left)
    { 
        _SHIFT;

        buff_pos += sprintf(curr_pos, "Node_%hhu:<left>->Node_%hhu;\n",
                            (unsigned char)((long)node & 0xff),
                            (unsigned char)((long)node->left & 0xff));

        buff_pos += dump_node_dot(node->left);
    }

    if(node->right)
    {
        _SHIFT;

        buff_pos += sprintf(curr_pos, "Node_%hhu:<right>->Node_%hhu;\n",
                            (unsigned char)((long)node & 0xff),
                            (unsigned char)((long)node->right & 0xff));

        buff_pos += dump_node_dot(node->right);
    }

    return buff_pos;
}


void dump_tree_dot(const char* output, Tree* tree)
{
    assert(output && "NOT Valid output file");

    static char dump_buff[8192] = {};
    static int buff_pos = 0;
    static int shift = 0;

    buff_pos += sprintf(curr_pos , "digraph tree{\n\trankdir=HR;\n");
    shift++;
    
    buff_pos += dump_node_dot(tree->root);

    buff_pos += sprintf(curr_pos, "}");
    FILE* fp = fopen(output, "w");

    fwrite(dump_buff, buff_pos, sizeof(char), fp);
    fclose(fp);
}
#undef dump_node
#undef curr_pos
#undef _SHIFT
