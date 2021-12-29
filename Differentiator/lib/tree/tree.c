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
    res->size = 0;

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
    if((*tree)->root != NULL)
    {
        node_free((*tree)->root->left);
        node_free((*tree)->root->right);
    }
    free((*tree)->root);
    if((*tree)->loaded)
        free((*tree)->text);
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

static int _save_node(Node* node, char* dump_buff, int buff_pos, int shift, int side)
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
    else if((node->type == VAR))
        buff_pos += sprintf(curr_pos, "value: \"%c\"\n", (int)(node->value.num));
    else if((node->type == OPER) || (node->type == FUNC)) 
        buff_pos += sprintf(curr_pos, "value: \"%s\"\n", (node->value.text));
    else
    {
        pr_err(LOG_CONSOLE, "Bad node type\n");
        return -1;
    }

    if(node->left != NULL)
    {
        int ret = save_node(node->left, -1);

        if(ret < 0)
        {
            pr_err(LOG_CONSOLE, "Error occured while saving\n");
            return -1;
        }

        buff_pos += ret;
    }

    if(node->right != NULL)
    {
        int ret = save_node(node->right, 1);

        if(ret < 0)
        {
            pr_err(LOG_CONSOLE, "Error occured while saving\n");
            return -1;
        }

        buff_pos += ret;
    }

    shift--;

    _SHIFT;

    buff_pos += sprintf(curr_pos, "}\n");

    return buff_pos - ret;
}

void save_tree(const char* output, Tree* tree)
{
    if(tree == NULL)
    {
        pr_err(LOG_CONSOLE, "bad tree to save: pointer is NULL\n");
        return;
    }

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

static bool is_func(char* txt)
{
    #define func(name)\
        if(strncasecmp(txt, #name, sizeof(#name) - 1) == 1)\
            return true;\
        else
    
    #include "func"

    #undef func
        return false;
}

static inline void _skip_spaces(char** txt)
{
    while(isspace(**(txt))) 
    {
        //fprintf(stderr, "skipping space (%lc)\n", **txt);
        (*txt)++;
    }
}

static inline void _skip_digits(char** txt)
{
    while(isdigit(**(txt))) 
    {
        //fprintf(stderr, "skipping space (%lc)\n", **txt);
        (*txt)++;
    }
}

static inline void _skip_chars(char** txt)
{
    while(isalpha(**(txt)))
    {
        //fprintf(stderr, "skipping char (%lc)\n", **txt);

        (*txt)++;
    }
}

static inline void _skip_alnum(char** txt)
{
    while(isalnum(**(txt)))
    {
        //fprintf(stderr, "skipping char (%lc)\n", **txt);

        (*txt)++;
    }
}
static inline void _skip_till(char** txt, char chr)
{
    while(**txt != chr) (*txt)++;
}

#define _SKIP_SPACES(txt) _skip_spaces(&txt);
#define _SKIP_CHARS(txt) _skip_chars(&txt);
#define _SKIP_TILL(txt, chr) _skip_till(&txt, chr);
#define _SKIP_DIGITS(txt) _skip_digits(&txt);
#define _SKIP_ALNUM(txt) _skip_alnum(&txt);
#define COMP_STR(txt, str) strncmp(txt, str, sizeof(str) - 1)
#define IS_FUNC(txt) is_func(txt)
#define PASS_2_FIELD_VAL(field)\
    _SKIP_SPACES(txt);\
    if(COMP_STR(txt, field) != 0)\
    {\
        free(node);\
        pr_err(LOG_CONSOLE, "Bad node format\n");\
        return NULL;\
    }\
    _SKIP_TILL(txt, ' ');\
    txt++;\

#define CHECK_COND(cond, msg)\
    if(cond)\
    {\
        free(node);\
        pr_err(LOG_CONSOLE, "Bad node format: "msg"\n");\
        return NULL;\
    }

#define REQUIRE(chr)\
    if(*txt != chr)\
    {\
        free(node);\
        pr_err(LOG_CONSOLE, "Bad .tr file format "\
                            "[expected \"%c\" but got \"%c\"]\n", chr, *txt);\
        return NULL;\
    }

#define IS_OPERATOR(c)\
    ((c == '+') || (c == '-') || (c == '*') || (c == '/'))

Node* parse_node_from_save(Tree* tree, char** text)
{
    char* txt = *text;

    tree->size++;
    
    Node* node = create_empty_node(); 

    REQUIRE('{');
    
    txt++;

    PASS_2_FIELD_VAL("type: ");

    CHECK_COND(isdigit(*txt) == 0, "bad \"type\" value");

    sscanf(txt, "%d", (int*)&(node->type));

    _SKIP_ALNUM(txt);

    PASS_2_FIELD_VAL("value: ");

    if(*txt == '"')
    {
        txt++;

        if(node->type == OPER)
        {
            if(!IS_OPERATOR(*txt))
            {
                pr_err(LOG_CONSOLE, "Bad \"value\" value: type is \"operator\""
                                    " but \"value\" is wrong\n", node->type);
                free(node);
                return NULL;
            }

            node->value.text = txt;
            _SKIP_TILL(txt, '"');
            *txt = '\0';
            txt++;
        }
        else if(node->type == FUNC)
        {
            if(!IS_FUNC(txt))
            {
                free(node);
                pr_err(LOG_CONSOLE, "Bad \"value\" value: type is \"function\""
                                    " but \"value\" is wrong\n", node->type);
                return NULL;
            }

            node->value.text = txt;
            _SKIP_TILL(txt, '"');
            *txt = '\0';
            txt++;
        }
        else if(node->type == VAR)
        {
            char temp = -1;
            sscanf(txt, "%c", &temp);
            node->value.num = temp;
            _SKIP_TILL(txt, '"');
            txt++;
        }
        else
        {
            //printf("%s\n", txt); 
            free(node);
            pr_err(LOG_CONSOLE, "Bad node type [%d is unknown]\n", node->type);\
            return NULL;
        }
    }
    else if(isdigit(*txt))
    {
        sscanf(txt, "%lf", &(node->value.num));

        _SKIP_DIGITS(txt);

        if((*txt == ',') && (isdigit(*(++txt)) != 0))
            _SKIP_DIGITS(txt);
    }
    else
    {
        free(node);
        pr_err(LOG_CONSOLE, "Bad node format: bad \"value\" value\n");\
        return NULL;
    }

    _SKIP_SPACES(txt);

    if(*txt == '}')
    {
        *text = txt;
        return node;
    }

    REQUIRE('l');
    txt++;
    REQUIRE('{');

    if(*txt == '{')
    {
        node->left = parse_node_from_save(tree, &txt);

        if(node->left == NULL)
        {
            pr_err(LOG_CONSOLE, "Bad node format\n");\
            return NULL;
        }

        node->left->parent = node;
    }

    txt++;
    _SKIP_SPACES(txt);
    REQUIRE('r');
    txt++;
    REQUIRE('{');

    if(*txt == '{')
    {
        node->right = parse_node_from_save(tree, &txt);

        if(node->right == NULL)
        {
            pr_err(LOG_CONSOLE, "Bad node format\n");\
            return NULL;
        }

        node->right->parent = node;
    }

    return node;
}

#undef _SKIP_SPACES
#undef _SKIP_CHARS
#undef _SKIP_TILL
#undef _SKIP_DIGITS
#undef IS_OPERATOR
#undef IS_FUNC
#undef COMP_STR
#undef _SKIP_ALNUM
#undef PASS_2_FIELD_VAL
#undef CHECK_COND
#undef REQUIRE

Tree* load_tree(const char* input)
{
    int sz = fileSize(input);

    if(sz < 0)
    {
        freopen(NULL, "w", stdout);
        pr_err(LOG_CONSOLE, "Bad input filepath\n");
        freopen(NULL, "w", stdout);
        return NULL;
    }
    printf("sz = %d\n", sz);

    char* txt = readText(input, sz);
    //fprintf(stderr, "File was read\n%ls\n", txt);

    Tree* tree = NULL;

    if(*txt == '{')
    {
        tree = (Tree*)calloc(1, sizeof(Tree));
        assert(tree != NULL);
        tree->size = 0;
        tree->root = NULL;

        tree->root = parse_node_from_save(tree, &txt);

        if(tree->root == NULL)
        {
            tree_free(&tree);
            free(txt);
            pr_err(LOG_CONSOLE, "Error occured while loading\n");
            return NULL;
        }
    }
    else
        pr_err(LOG_CONSOLE_STDERR,  "Bad .tr file format"
                                    " [expected \"%c\", got \"%c\"]\n",
                                    '{', *txt); 
    tree->loaded = true;
    tree->text = txt;

    return tree;
}
