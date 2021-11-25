#include "tree.h"
#include <stdlib.h>
#include <wchar.h>
#include <assert.h>
#include <string.h>
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

Node* wcreate_node(wchar_t* str)
{
    
    Node* res = (Node*)calloc(1, sizeof(Node));

    res->parent = res->left = res->right = NULL;
    res->str = wcsdup(str);

    return res;
}

void node_free(Node* node)
{
    if(node == NULL)
        return;

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

static int _shift(void* buff, int shift)
{
    int ret = 0;
    for(; shift > 0;  ret++, shift--)
        sprintf((char*)(buff + ret), "%c", '\t');

    return ret;
}

#define _SHIFT buff_pos += _shift(dump_buff + buff_pos, shift);
#define dump_node_dot(node) _dump_node_dot(node, dump_buff, buff_pos, shift);
#define curr_pos (dump_buff + buff_pos)

#define _PRINT_STR(str)\
    buff_pos += sprintf(curr_pos,  "\"");\
    buff_pos += wcstombs((char*)curr_pos, str, LOG_SIZE - buff_pos);\
    buff_pos += sprintf(curr_pos, "\"");

#define LOG_SIZE 8192

static int _dump_node_dot(Node* node, char* dump_buff, int buff_pos, int shift)
{
    int ret = buff_pos;

    _SHIFT;
    
    _PRINT_STR(node->str);

    buff_pos += sprintf(curr_pos, "[shape=egg];\n");
    
    if(node->parent != NULL)
    {
        _SHIFT;

        _PRINT_STR(node->str);

        buff_pos += sprintf(curr_pos, "->");

        _PRINT_STR(node->parent->str);

        buff_pos += sprintf(curr_pos, "[color=\"red\"];\n");
    }


    if(node->left != NULL)
    { 
        _SHIFT;

        _PRINT_STR(node->str);
        buff_pos += sprintf(curr_pos, "->");
        _PRINT_STR(node->left->str);
        buff_pos += sprintf(curr_pos, "[label=\"Да\"];\n");

        /*
        buff_pos += sprintf(curr_pos,  "Node_%hhu:<left>->Node_%hhu;\n",
                            (unsigned char)((long)node & 0xff),
                            (unsigned char)((long)node->left & 0xff));

        */
        buff_pos += dump_node_dot(node->left);
    }

    if(node->right != NULL)
    {
        _SHIFT;

        _PRINT_STR(node->str);
        buff_pos += sprintf(curr_pos, "->");
        _PRINT_STR(node->right->str);
        buff_pos += sprintf(curr_pos, "[label=\"Нет\"];\n");

        /*
        buff_pos += sprintf(curr_pos,  "Node_%hhu:<right>->Node_%hhu;\n",
                            (unsigned char)((long)node & 0xff),
                            (unsigned char)((long)node->right & 0xff));
        */

        buff_pos += dump_node_dot(node->right);
    }

    return buff_pos - ret;
}


void dump_tree_dot(const char* output, Tree* tree)
{
    assert(output && "NOT Valid output file");

    static char dump_buff[LOG_SIZE] = {};
    static int buff_pos = 0;
    static int shift = 0;

    buff_pos += sprintf(curr_pos ,  "digraph tree{\n\trankdir=HR;\n");
    shift++;
    
    buff_pos += dump_node_dot(tree->root);

    buff_pos += sprintf(curr_pos,  "}");

    FILE* fp = fopen(output, "w");
    assert(fp != NULL);

    fwrite(dump_buff, buff_pos, sizeof(char), fp);
    //fwprintf(fp, L"%s", dump_buff);
    fclose(fp);

    memset(dump_buff, '\0', buff_pos);
    buff_pos = 0;
    shift = 0;
}

#define save_node(node) _save_node(node, dump_buff, buff_pos, shift)

int _save_node(Node* node, char* dump_buff, int buff_pos, int shift)
{
    int ret = buff_pos;

    _SHIFT;

    buff_pos += sprintf(curr_pos, "{\n");
    shift++;

    _SHIFT;
    
    _PRINT_STR(node->str);

    buff_pos += sprintf(curr_pos, "\n");

    if(node->left != NULL)
        buff_pos += save_node(node->left);

    if(node->right != NULL)
        buff_pos += save_node(node->right);

    shift--;

    _SHIFT;

    buff_pos += sprintf(curr_pos, "}\n");

    return buff_pos - ret;
}

void save_tree(const char* output, Tree* tree)
{
    static char dump_buff[LOG_SIZE] = {};
    static int buff_pos = 0;
    static int shift = 0;

    buff_pos += save_node(tree->root);

    FILE* fp = fopen(output, "w");
    assert(fp != NULL);

    fwrite(dump_buff, buff_pos, sizeof(char), fp);

    fclose(fp);

    memset(dump_buff, '\0', buff_pos);
    buff_pos = 0;
    shift = 0;

}

#undef save_node
#undef LOG_SIZE
#undef _PRINT_STR
#undef curr_pos
#undef dump_node_dot
#undef _SHIFT
/*
Node* parse_node(
Tree* load_tree(const char* input)
{
    int sz = fileSize(input);

    wchar_t* txt = readText(input, size);

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));

    if(*txt == L'{')
        tree->root = parse_node(txt);
}
*/
