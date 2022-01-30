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


typedef struct
{
    char* name;
    int num_of_args;
} func_prop_t;


typedef enum
{
    OK,
    NOT_OK
} status_t;

status_t status;
static char for_var[20] = {};


bool is_in_list(List* list, char* name)
{
    for(int i = 1; i <= list->size; i ++)
        if(strncmp(((func_prop_t*)ListGetElement(list, i))->name, name, strlen(name)) == 0)
            return true;

    return false;
}

bool is_known_func(char* func)
{
#define func(name)\
    if(strncmp(func, #name, sizeof(#name) - 1) == 0)\
        return true;\
    else

    #include "known_func"
#undef func

    if(is_in_list(&Id_list, func))
        return true;
    else
        return false;
}



void SyntaxError(const char* format, ...)
{
    5+4;
    int x;
    x;
    va_list params;
    va_start(params, format);

    setColor(NULL, FG_RED);
    printf("Syntax error: ");
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

// G ::= {Func_def}+ '$' 
Node* GetG(const char* str)
{
    s = str;

    skipSpaces(s);

    Node* res;

    Node* val = GetFunc_def();
    res = val;

    check_status(val);

    //PrintList(&Id_list);
    skipSpaces(s);

    while(*s != '$')
    {
        skipSpaces(s);
        Node* val2 = GetFunc_def();
        check_status(val2);
        attach_node(val, right, val2);
        val = val2;
        //PrintList(&Id_list);
    }

    REQUIRE('$', *s);
    return res;
}

// E ::= T{[+-]T}*
Node* GetE()
{
    pr_info(LOG_CONSOLE, "Parsing Expression\n\t\"%.10s...\"\n", s);

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
    pr_log(LOG_CONSOLE, "Returning from GetE();\n\t\"%.5s\"\n", s);
    return val1;
}

// T ::= P{[*/]P}*
Node* GetT()
{
    pr_info(LOG_CONSOLE, "Parsing terminals\n\t\"%.10s...\"\n", s);
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

    pr_log(LOG_CONSOLE, "Returning from GetT();\n\t\"%.5s\"\n", s);
    return val1;
}

// F ::= {"exp" | "ln" | "sin" | "cos" | ... } '('E {',' E}*')'
Node* GetFunc_call()
{
    skipSpaces(s);
    char* func_name = GetId();

    if(func_name == NULL)
        return NULL;

    if(!is_known_func(func_name))
    {
        free(func_name);
        return NULL;
    }

    skipSpaces(s);
    
    REQUIRE('(', *s);

    skipSpaces(s);

    //let it be without parameters for a while
    /*
    Node* body = GetE();
    check_status(body);

    skipSpaces(s)
    */

    REQUIRE(')', *s);
    Node* res = create_func_call_node(func_name);
    res->alloc = true;
    return res;
}

// P ::= '('E')' | N | F | Id
Node* GetP()
{
    Node* val = NULL;

    pr_info(LOG_CONSOLE, "Parsing Primary Expression\n\t\"%.10s...\"\n", s);

    if(*s == '(')
    {
        s++;
        skipSpaces(s);

        pr_log(LOG_CONSOLE, "Parsing Expression\n");
        val = GetE();

        check_status(val);

        skipSpaces(s);
        REQUIRE(')', *s);
        s++;
        pr_log(LOG_CONSOLE, "Returning from GetP();\n\t\"%.5s\"\n", s);
        return val;
    }


    pr_log(LOG_CONSOLE, "Parsing Number\n\t\"%.10s...\"\n", s);
    if((val = GetN()) != NULL)
    {
        pr_log(LOG_CONSOLE, "Returning from GetP();\n\t\"%.5s\"\n", s);
        return val;
    }

    pr_log(LOG_CONSOLE, "Parsing Function call\n\t\"%.10s...\"\n", s);
    if((val = GetFunc_call()) != NULL)
    {
        pr_log(LOG_CONSOLE, "Returning from GetP();\n\t\"%.5s\"\n", s);
        return val;
    }

    char* id = NULL;
    if((id = GetId()) != NULL)
        if(is_in_id_table(current_id_table, id))
        {
            pr_log(LOG_CONSOLE, "Returning from GetP();\n\t\"%.5s\"\n", s);
            Node* res = create_variable_node(id);
            res->alloc = true;
            return res;
        }
    
    pr_err(LOG_STDERR, "Error occured\n");
    exit(EXIT_FAILURE);
}

Node* GetN()
{
    int val = 0;

    skipSpaces(s);

    const char* begin = s;

    if(!isdigit(*s))
    {
        status = NOT_OK;
        //SyntaxError("Expected number\n");
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
    pr_info(LOG_CONSOLE, "Parsing Assignement\n\t\"%.10s...\"\n", s);
    skipSpaces(s);

    char* id = GetId();

    if(id == NULL)
        return NULL;

    skipSpaces(s);

    if(*s != '=')
    {
        free(id);
        return NULL;
    }

    s++;
    // TODO
    // refactoring is needed here
    //
    //

    bool flag = false;
    if(!is_in_id_table(current_id_table, id))
    {
        flag = true;
        id_record_t* last_record = get_last_record(current_id_table);

        if(last_record == NULL)
            add_id_record(current_id_table, (id_record_t){id, 0});
        else
            add_id_record(current_id_table, 
                      (id_record_t){id, last_record->offset + 4});
    }
    /*
    else //it is already in the table, so we do not need the name
        //pr_err(LOG_CONSOLE, "Var is %s\n", id);
        free(id);
    */

    skipSpaces(s);

    Node* expr = GetE();

    check_status(expr);
    
    Node* res = create_oper_node('=');

    attach_node(res, left, create_variable_node(id));
    if(!flag)
        res->left->alloc = true;
    attach_node(res, right, expr);

    pr_log(LOG_CONSOLE, "Returning from GetAssign();\n\t\"%.5s\"\n", s);
    return res;
}

// If ::= "if" '('E')' Blk | St  [let it be without "else" for now] {"else" Blk | St}?
Node* GetIf()
{
    pr_info(LOG_CONSOLE, "Parsing \"If\" condition \n\t\"%.10s...\"\n", s);
    skipSpaces(s);

    if(strncmp(s, "if", sizeof("if") - 1) != 0)
        return NULL;

    s += sizeof("if") - 1; //skipping "if"

    skipSpaces(s);

    REQUIRE('(', *s);

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

    pr_log(LOG_CONSOLE, "Returning from GetIf();\n\t\"%.5s\"\n", s);
    return res;
}

// St ::= If | { Assign | E ';'}
Node* GetSt()
{
    pr_info(LOG_CONSOLE, "Parsing Statement\n\t\"%.10s...\"\n", s);
    skipSpaces(s);

    Node* res;

    const char* begin = s;
    Node* val = GetIf();

    if(val != NULL)
    {
        res = create_empty_node();
        attach_node(res, left, val);
        pr_log(LOG_CONSOLE, "Returning from GetSt();\n\t\"%.5s\"\n", s);
        return res;
    }


    res = create_semicolon_node();

    val = GetAssign();

    if(val != NULL)
    {
        skipSpaces(s);
        REQUIRE(';', *s);
        attach_node(res, left, val);
        pr_log(LOG_CONSOLE, "Returning from GetSt();\n\t\"%.5s\"\n", s);
        return res;
    }

    s = begin; // restoring string after wrong GetAssignement

    val = GetE();
    if(val != NULL)
    {
        skipSpaces(s);
        REQUIRE(';', *s);
        attach_node(res, left, val);
        pr_log(LOG_CONSOLE, "Returning from GetSt();\n\t\"%.5s\"\n", s);
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
    pr_info(LOG_CONSOLE, "Parsing Block of statements\n\t\"%.10s...\"\n", s);

    if(*s != '{')
        return NULL;
    s++;
    
    Node* res;

    Node* val1 = GetSt();
    res = val1;

    check_status(val1); 

    skipSpaces(s);

    while(*s != '}')
    {
        Node* val2 = GetSt();
        check_status(val2);
        attach_node(val1, right, val2);
        //val2->right = val1;
        val1 = val2;
        skipSpaces(s);
    }
    s++;
    return res;
}

// Func_def ::= Id'(' Id{',' V}* ')' Blk
Node* GetFunc_def()
{
    char* id =  GetId();

    pr_info(LOG_CONSOLE, "new function name \"%s\"\n", id);

    if(id == NULL)
        return NULL;

    if(*s != '(')
    {
        free(id);
        return NULL;
    }

    s++;
    
    Node* res = create_func_node(id);
    res->alloc = true;
    res->id_table = (id_table_t*)calloc(1, sizeof(id_table_t));

    int offset = 0;
    int num_of_args = 0;

    skipSpaces(s);

    char* var = GetId();

    if(var != NULL)
    {
        pr_log(LOG_CONSOLE, "adding new id_record with \"%s\" name\n", var);

        add_id_record(res->id_table, (id_record_t){var, offset});
        num_of_args++;

        skipSpaces(s);

        while(*s == ',')
        {
            s++;
            offset += 4;
            skipSpaces(s);
            pr_log(LOG_CONSOLE, "parsing new parameter \"%.10s\"\n", s);

            var = GetId();
            pr_log(LOG_CONSOLE, "adding new id_record with \"%s\" name\n", var);

            if(var == NULL)
            {
                pr_err(LOG_STDERR, "Syntax error: function definiton\n");
                exit(EXIT_FAILURE);
            }
            add_id_record(res->id_table, (id_record_t){var, offset});
            num_of_args++;
            skipSpaces(s);
        }
    }
    else
        skipSpaces(s);
    
    REQUIRE(')', *s);


    func_prop_t* func_prop = (func_prop_t*)calloc(1, sizeof(func_prop_t));
    func_prop->name = id;
    func_prop->num_of_args = num_of_args;

    pr_warn(LOG_CONSOLE, "Inserted prop of func \"%s\""
                        " [name = %s, num of args = %d]\n",
                        id, id, num_of_args);

    ListInsertBack(&Id_list, func_prop);

    current_id_table = res->id_table;

    skipSpaces(s);

    Node* func_body = GetBlk();

    if(func_body == NULL)
    {
        pr_err(LOG_STDERR, "Syntax error: function definiton\n");
        exit(EXIT_FAILURE);
    }

    attach_node(res, left, func_body);
    //res->left = func_body;

    current_id_table = NULL;
    return res;
}

void free_list_elements(List* list)
{
    for(int i = 1; i <= list->size; i++)
    {
        //func_prop_t* f_p = (func_prop_t*)ListGetElement(list, i);
        //free(f_p->name);
        //free(f_p);
        free(ListGetElement(list, i));
    }
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
    dump_tree_dot("ggff", tree);

    tree_free(&tree);

    free_list_elements(&Id_list);
    ListDtor(&Id_list);

    //fprintf(stdout, "Result is: %d\n", GetG(argv[1]));
    return 0;
}
