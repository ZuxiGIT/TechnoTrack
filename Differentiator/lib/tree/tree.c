#include "tree.h"
#include "../logger/logger.h"
#include "../TextLib/File.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

Tree* tree_init()
{
    Tree* res = (Tree*)calloc(1, sizeof(Tree)); 

    res->root = NULL; 
    res->size = 1;

    //res->root->left = res->root->right = res->root->parent = NULL;

    return res;
}

Node* _create_node(type_t type, value_t value, Node* parent, Node* left, Node* right)
{
    Node* res = (Node*)calloc(1, sizeof(Node));

    res->value = value;
    res->parent = parent;
    res->left = left;
    res->right = right;
    res->type = type;

    return res;
}

inline Node* create_empty_node()
{
    //Node* res = (Node*)calloc(1, sizeof(Node));
    //
    //res->value.text = NULL;
    //res->parent = res->left = res->right = NULL;
    //res->type = UNDEFINED;
    //
    //return res;
    return _create_node(UNDEFINED, (value_t)((char*)NULL), NULL, NULL, NULL);
}

inline Node* create_node(type_t type, value_t value)
{
    return _create_node(type, (value_t) value, NULL, NULL, NULL);
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

    free(node);

    return;
}

void tree_free(Tree** tree)
{
    node_free((*tree)->root->left);
    node_free((*tree)->root->right);
    free((*tree)->root);
    free(*tree);
    *tree = NULL;
}

static inline int _shift(void* buff, int shift)
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

/*
static int _dump_node_dot(Node* node, char* dump_buff, int buff_pos, int shift)
{
    int ret = buff_pos;

    _SHIFT;
    
    //_PRINT_STR(node->str);
    buff_pos += sprintf(curr_pos,  "\"");\
    //fprintf(stderr, "--------> output str: %ls\n", node->str);
    buff_pos += wcstombs((char*)curr_pos, node->str, LOG_SIZE - buff_pos);\
    buff_pos += sprintf(curr_pos, "\"");

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

        
        buff_pos += sprintf(curr_pos,  "Node_%hhu:<left>->Node_%hhu;\n",
                            (unsigned char)((long)node & 0xff),
                            (unsigned char)((long)node->left & 0xff));

        
        buff_pos += dump_node_dot(node->left);
    }

    if(node->right != NULL)
    {
        _SHIFT;

        _PRINT_STR(node->str);
        buff_pos += sprintf(curr_pos, "->");
        _PRINT_STR(node->right->str);
        buff_pos += sprintf(curr_pos, "[label=\"Нет\"];\n");

        
        buff_pos += sprintf(curr_pos,  "Node_%hhu:<right>->Node_%hhu;\n",
                            (unsigned char)((long)node & 0xff),
                            (unsigned char)((long)node->right & 0xff));
        

        buff_pos += dump_node_dot(node->right);
    }

    return buff_pos - ret;
}


void dump_tree_dot(const char* output, Tree* tree)
{
    assert(output && "NOT Valid output file");
    assert(tree && "NOT Valid tree (NULL)");

    static char dump_buff[LOG_SIZE] = {};
    static int buff_pos = 0;
    static int shift = 0;

    buff_pos += sprintf(curr_pos ,  "digraph tree{\n\trankdir=HR;\n");
    shift++;
    
    buff_pos += dump_node_dot(tree->root);

    buff_pos += sprintf(curr_pos,  "}");

    FILE* fp = fopen(output, "w");
    wprintf(L"output file is %s\n", output);
    assert(fp != NULL);

    fwrite(dump_buff, buff_pos, sizeof(char), fp);
    //fwprintf(fp, L"%s", dump_buff);
    fclose(fp);

    memset(dump_buff, '\0', buff_pos);
    buff_pos = 0;
    shift = 0;
}
*/
#define save_node(node, side) _save_node(node, dump_buff, buff_pos, shift, side)
/*
 *
 * side ==  1  ---> left child
 * side ==  0  ---> root node (it has children, but no parent :( )
 * side == -1  ---> rigth child
 *
 */

int _save_node(Node* node, char* dump_buff, int buff_pos, int shift, int side)
{
    int ret = buff_pos;

    _SHIFT;

    if(side == -1)
        buff_pos += sprintf(curr_pos, "l");
    else if(side == 1)
        buff_pos += sprintf(curr_pos, "r");

    buff_pos += sprintf(curr_pos, "{\n");

    shift++;

    _SHIFT;
    
    buff_pos += sprintf(curr_pos, "type: %d\n", node->type);

    _SHIFT;

    if(node->type == CONST) 
        buff_pos += sprintf(curr_pos, "value: %lf\n", node->value.num);
    else
        buff_pos += sprintf(curr_pos, "value: \"%c\"\n", (*(node->value.text)));


    if(node->left != NULL)
        buff_pos += save_node(node->left, -1);

    if(node->right != NULL)
        buff_pos += save_node(node->right, 1);


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

    buff_pos += save_node(tree->root, 0);

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

#define _SKIP_SPACES(txt) _skip_spaces(&txt);
#define _SKIP_CHARS(txt) _skip_chars(&txt);
#define _SKIP_TILL(txt, chr) _skip_till(&txt, chr);
#define REQUIRE(chr)\
    if(*txt != chr)\
    {\
        pr_err(LOG_CONSOLE_STDERR, "Bad .tr file format"\
                                    " (expected [%c] but got [%c])\n", chr, *txt);\
        abort();\
        return NULL;\
    }

#define IS_OPERATOR(c)\
    (c == '+') || (c == '-') || (c == '*') || (c == '/')

Node* parse_node(Tree* tree, char** text)
{
    char* txt = *text;

    tree->size++;
    
    Node* node = create_empty_node(); 

    if(*txt == '(')
    {
        node->left = parse_node(tree, &txt);
        node->left->parent = node;
    }
    
    txt++;

    if(isdigit(*txt))
    {
        node->type = CONST;
        sscanf(txt, "%lf", &(node->value.num));
    }
    else if(isalpha(*txt))
    {
        node->type = VAR;
        //sscanf(txt, "%c", &(temp));
        node->value.text = txt;
    }
    else if(IS_OPERATOR(*txt))
    {
        node->type = OPER;
        //sscanf(txt, "%c", &(temp));
        node->value.text = txt;
    }
    else if(*txt == '#')
    {
        free(node);
        return NULL;
    }

    txt++;

 
    if(*txt == '(')
    {
        node->right = parse_node(tree, &txt);
        node->right->parent = node;
    }
    else if(node->left != NULL)
        REQUIRE('(');

    txt++;
    REQUIRE(')');

    return node;
}


    
#undef _SKIP_CHARS
#undef _SKIP_TILL
#undef REQUIRE

Tree* parse_tree_from_source(const char* input)
{
    int sz = fileSize(input);

    if(sz < 0)
    {
        freopen(NULL, "w", stdout);
        pr_err(LOG_CONSOLE, "Bad input file\n");
        freopen(NULL, "w", stdout);
        return NULL;
    }

    char* txt = readText(input, sz);
    //fprintf(stderr, "File was read\n%ls\n", txt);

    Tree* tree = NULL;

    if(*txt == '(')
    {
        tree = (Tree*)calloc(1, sizeof(Tree));
        assert(tree != NULL);
        tree->size = 0;
        tree->root = NULL;

        txt++;

        tree->root = parse_node(tree, &txt);
    }
    else
        pr_err(LOG_CONSOLE_STDERR,  "Bad .tr file format"
                                    " [expected %c, got %c]\n",
                                    '{', *txt); 
    return tree;
}
