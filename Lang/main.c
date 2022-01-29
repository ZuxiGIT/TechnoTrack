#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#include "./lib/logger/TColors.h"
#include "./lib/logger/logger.h"

#include "./lib/tree/DSL.h"
#include "./lib/tree/tree.h"


#define check_status(node)\
do{\
    if(node == NULL)\
    {\
        pr_err(LOG_CONSOLE, "Error occured\n");\
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
// G ::= {St}+ '$'                                                  --> gramatic
// E ::= T{[+-]T}*                                                  --> expresion
// T ::= P{[*/]P}*                                                  --> 
// P ::= '('E')' | N | F | N                                        --> primary expresion
// N ::= [0-9]+                                                     --> number
// V ::= [_a-zA-Z]{[_a-zA-z0-9]}*                                   --> variable
// Op ::= [+-*/%<<>>]                                               --> operator
// Com_op ::= Op | {=}?                                             --> compound operator
// Assign ::= V'='E                                                 --> assignemnet
// F ::= {"exp" | "ln" | "sin" | "cos" | ... } '('E {',' E}*')'     --> function call
// St ::= If { Assign | E ';'}                                      --> statement
// Blk ::= '{' St + '}'                                             --> usual block of statements
// If ::= "if"'('E')'Blk | St {"else" Blk | St}                     --> if condition


// old gramatic
// G ::= E'$'
// E ::= T{[+-]T}*
// T ::= P{[*/]P}*
// P ::= '(E')' | N
// N ::= [0-9]+

typedef enum
{
    OK,
    NOT_OK
} status_t;

status_t status;
static char for_var[20] = {};


void SyntaxError(const char* format, ...)
{
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
        pr_err(LOG_CONSOLE, "Error occured\n");
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
            pr_err(LOG_CONSOLE, "Error occured\n");
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

    pr_log(LOG_CONSOLE, "New variable \"%s\"\n", var);
    memcpy(for_var, var, len);

    return for_var;
}

int main(int argc, char **argv)
{
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
