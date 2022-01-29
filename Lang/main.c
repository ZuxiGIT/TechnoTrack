#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#include "./lib/list/list.h"

#include "./lib/id_table/id_table.h"

#include "./lib/logger/TColors.h"
#include "./lib/logger/logger.h"

#include "./lib/tree/DSL.h"
#include "./lib/tree/tree.h"


#define check_status(node)\
do{\
    if(node == NULL)\
    {\
        pr_err(LOG_STDERR, "Error occured\n");\
        exit(EXIT_FAILURE);\
    }\
}while(0);

#define skipSpaces(txt) _skip_spaces(&txt);
static inline void _skip_spaces(const char** txt)
{
    while(isspace(**(txt))) 
    {
        (*txt)++;
    }
}

#define REQUIRE(req, got)\
    if(*s == req) s++;\
    else \
    { \
        pr_err(LOG_STDERR, "bad string, got \"%c\", expected \"%c\"\n",\
                            got, req);\
        exit(1);\
    }

// current gramatic
// G ::= {Func_def}+ '$'                                            --> gramatic
// St ::= If | { Assign | E ';'}                                    --> statement
// If ::= "if" '('E')' Blk | St {"else" Blk | St}?                  --> if condition 
// Blk ::= '{' {St} + '}'                                           --> usual block of statements
// E ::= T{[+-]T}*                                                  --> expresion
// T ::= P{[*/]P}*                                                  --> 
// P ::= '('E')' | N | F | Id                                       --> primary expresion
// N ::= [0-9]+                                                     --> number
// Id ::= [_a-zA-Z]{[_a-zA-z0-9]}*                                  --> identity 
// Op ::= [+-*/%<<>>]                                               --> operator
// Com_op ::= Op'='                                                 --> compound operator
// Assign ::= Id'='E                                                --> assignemnet
// F ::= {"exp" | "ln" | "sin" | "cos" | ... } '('E {',' E}*')'     --> function call
// Func_def ::= Id'(' Id{',' V}* ')' Blk                            --> function definition
// Var_def ::= Id'='N'

// old gramatic
// G ::= E'$'
// E ::= T{[+-]T}*
// T ::= P{[*/]P}*
// P ::= '(E')' | N
// N ::= [0-9]+

List Id_list;
id_table_t* current_id_table;

typedef enum
{
    OK,
    NOT_OK
} status_t;

status_t status;
static char for_var[20] = {};


void SyntaxError(const char* format, ...)
{
    5+4;
    int x;
    x;
    va_list params;
    va_start(params, format);

    setColor(NULL, FG_RED);
    printf("Synax error: ");
    resetColor(NULL);

    vprintf(format, params);

    va_end(params);
}


Node* GetG(const char* str);
Node* GetE();
Node* GetT();
Node* GetP();
Node* GetN();
char* GetV();
char* GetId();
Node* GetAssign();
Node* GetSt();
Node* GetBlk();
Node* GetIf();
Node* GetFunc_def();



const char* s = NULL;

Node* GetG(const char* str)
{
    status = OK;
    s = str;
    skipSpaces(s);
    Node* val = GetE();
    check_status(val);
    skipSpaces(s);
    REQUIRE('$', *s);
    return val;
}

Node* GetE()
{
    skipSpaces(s);
    Node* val1 = GetT();
    skipSpaces(s);
    check_status(val1);

    while(*s == '+' || *s == '-')
    {
        char op = *s;
        s++;

        skipSpaces(s);
        Node* val2 = GetT();
        check_status(val2);
        skipSpaces(s);

        Node* node = create_oper_node(op);
        attach_node(node, left, val1);
        attach_node(node, right, val2);
        val1 = node;
    }
    return val1;
}

Node* GetT()
{
    skipSpaces(s);

    Node* val1 = GetP();
    if(val1 == NULL)
    {
        pr_err(LOG_STDERR, "Error occured\n");
        exit(EXIT_FAILURE);
    }

    skipSpaces(s);
    while(*s == '*' || *s == '/')
    {
        char op = *s;
        s++;

        skipSpaces(s);
        Node* val2 = GetP();
        skipSpaces(s);

        if(val2 == NULL)
        {
            pr_err(LOG_STDERR, "Error occured\n");
            exit(EXIT_FAILURE);
        }

        Node* node = create_oper_node(op);
        attach_node(node, left, val1);
        attach_node(node, right, val2);
        val1 = node;

    }

    return val1;
}

Node* GetP()
{
    Node* val = NULL;
    if(*s == '(')
    {
        s++;
        val = GetE();
        check_status(val);
        REQUIRE(')', *s);
    }
    else
    {
        val = GetN();
        check_status(val);
    }

    return val;
}

Node* GetN()
{
    int val = 0;

    skipSpaces(s);

    const char* begin = s;

    if(!isdigit(*s))
    {
        status = NOT_OK;
        SyntaxError("Expected number\n");
        return NULL;
    }

    while(isdigit(*s))
    {
        val = val * 10 + (*s - '0');
        s++;
    }

    return create_const_node(val);
}

char* GetV()
{
    static const int var_max_len = 20;

    char var[var_max_len];           //variable-sized object may not be initialized
    memset(var, '\0', var_max_len);

    int len = 0;
    if(isalpha(*s) != 0 || *s == '-')
        var[len++] = *s++;
    else
       SyntaxError("wrong string\n"); 

    while(isalpha(*s) != 0 || isdigit(*s) != 0 || *s == '-')
    {
        var[len] = *s++;
        len++;
    }

    pr_log(LOG_STDERR, "New variable \"%s\"\n", var);
    memcpy(for_var, var, len);

    return for_var;
}

char* GetId()
{
    const char* begin = s;
    while(isalpha(*s))
        s++;
    if(begin == s)
        return NULL;
    else
        return strndup(begin, s - begin);
}

// Assign ::= Id'='E
Node* GetAssign()
{
    skipSpaces(s);

    char* id = GetId();

    if(id == NULL)
        return NULL;

    skipSpaces(s);

    if(*s != '=')
        return NULL;
    else
    {
        if(!is_in_id_table(current_id_table, id))
            add_id_record(current_id_table, 
                          (id_record_t){id, get_last_record(current_id_table)->offset + 4});
        skipSpaces(s);
        Node* expr = GetE();

        check_status(expr);
    }
    
    Node* res = create_oper_node('=');

    attach_node(res, left, create_variable_node(id));
    attach_node(res, right, expr);

    return res;
}

// If ::= "if" '('E')' Blk | St  [let it be without "else" for now] {"else" Blk | St}?
Node* GetIf()
{
    skipSpaces(s);

    if(strncmp(s, "if", sizeof("if") - 1) != 0)
        return NULL;

    s += sizeof("if") - 1; //skipping "if"

    skipSpaces(s);

    REQUIRE('(', *s);

    s++;

    skipSpaces(s);

    Node* condition = GetE();

    check_status(condition);

    Node* res = create_condition_node();
    attach_node(res, left, condition);

    skipSpaces(s);

    REQUIRE(')', *s);

    skipSpaces(s);

    Node* body = GetBlk();

    if(body == NULL)
    {
        body = GetSt();
        check_status(body);
    }

    attach_node(res, right, body);

    return res;
}

// St ::= If | { Assign | E ';'}
Node* GetSt()
{
    skipSpaces(s);

    Node* res = create_semicolon_node();

    Node* val = GetIf();

    if(val != NULL)
    {
        res->left = val;
        return res;
    }


    val = GetAssign();

    if(val != NULL)
    {
        skipSpaces(s);
        REQUIRE(';', *s);
        s++;
        res->left = val;
        return res;
    }
    val = GetE();
    if(val != NULL)
    {
        skipSpaces(s);
        REQUIRE(';', *s);
        s++;
        res->left = val;
        return res;
    }
    else
    {
        pr_err(LOG_STDERR, "Error occured\n");
        exit(EXIT_FAILURE);
    }
}

// Blk ::= '{' {St} + '}'
Node* GetBlk()
{
    if(*s != '{')
        return NULL;
    s++;
    
    Node* val1 = GetSt();

    check_status(val); 

    skipSpaces(s);

    while(*s != '}')
    {
        Node* val2 = GetSt();
        check_status(val2);
        val2->right = val1;
        val1 = val2;
        skipsSpaces(s);
    }
    s++;
    return val1;
}

// Func_def ::= Id'(' Id{',' V}* ')' Blk
Node* GetFunc_def()
{
    char* id =  GetId();
    if(id == NULL)
        return NULL;

    if(*s != '(')
    {
        free(id);
        return NULL;
    }

    ListInsertBack(&Id_list, id); 
    s++;
    
    Node* res = create_func_node(id);
    res->id_table = (id_table_t*)calloc(1, sizeof(id_table_t));

    int offset = 0;
    skipSpaces(s);
    char* var = GetId();

    if(var != NULL)
    {
        add_id_record(res->id_table, (id_record_t){var, offset});
        skipSpaces(s);
        while(*s == ',')
        {
            offset += 4;
            skipSpaces(s);
            var = GetId();

            if(var == NULL)
            {
                pr_err(LOG_STDERR, "Syntax error: function definiton\n");
                exit(EXIT_FAILURE);
            }
            add_id_record(res->id_table, (id_record_t){var, offset});
            skipSpaces(s);
        }
    }
    else
        skipSpaces(s);
    
    REQUIRE(')', *s);

    current_id_table = res->id_table;

    Node* func_body = GetBlk();

    if(func_body == NULL)
    {
        pr_err(LOG_STDERR, "Syntax error: function definiton\n");
        exit(EXIT_FAILURE);
    }

    res->left = func_body;

    current_id_table = NULL;
    return res;
}


int main(int argc, char **argv)
{
    ListCtor(&Id_list, 20);

    if(argc < 2)
    {
        pr_err(LOG_STDERR, "no input\n");
        exit(1);
    }

    Tree* tree = tree_init();
    tree->root = GetG(argv[1]);
    dump_tree_dot("ff", tree);
    //fprintf(stdout, "Result is: %d\n", GetG(argv[1]));
    return 0;
}
