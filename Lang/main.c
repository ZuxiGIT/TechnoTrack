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
#include "./lib/tree/_tree.h"

#include "./lib/TextLib/File.h"


#define check_status(node)\
do{\
    if(node == NULL)\
    {\
        pr_err(LOG_STDERR, "Error occured\n");\
        exit(EXIT_FAILURE);\
    }\
}while(0);

#define skipAlphas(txt) _skip_alphas(&txt);
static inline void _skip_alphas(const char** txt)
{
    while(isalpha(**(txt))) 
    {
        (*txt)++;
    }
}
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
        pr_err(LOG_STDERR, "bad string, got \"%.1s\", expected \"%c\"\n",\
                            got, req);\
        exit(EXIT_FAILURE);\
    }

// current gramatic
// G ::= {Func_def | {Var_def ';'} }+ '$'                           --> gramatic
// St ::= If | { Var_def | Assign | E ';'}                          --> statement
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
// Var_def ::= "let" Id {'='E}? ';'

// old gramatic
// G ::= E'$'
// E ::= T{[+-]T}*
// T ::= P{[*/]P}*
// P ::= '(E')' | N
// N ::= [0-9]+

// only for functions and global variables
static List global_id_list;

// a list of id_tables of visible variables in the current scope`
static List id_table_list;

static char for_var[20] = {};

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

bool is_known_var(char* var)
{
    for(int i = 1; i <= id_table_list.size; i++)
        if(is_in_id_table(ListGetElement(&id_table_list, i), var))
            return true;

    for(int i = 1; i <= global_id_list.size; i++)
    {
        Id_node* node = ListGetElement(&global_id_list, i);
        if(strncmp(node->value.text, var, strlen(var)) == 0)
            return true;
    } 

    return false;
}

int is_in_list(List* list, char* name)
{
    for(int i = 1; i <= list->size; i ++)
        if(strncmp(((Id_node*)ListGetElement(list, i))->value.text, name, strlen(name)) == 0)
            return i;

    return 0;
}

bool is_known_func(char* func)
{
    int num_of_func;
#define func(name)\
    if(strncmp(func, #name, sizeof(#name) - 1) == 0)\
        return true;\
    else

    #include "known_func"
#undef func

    // currently function and variables cannot have the same name 
    if(num_of_func = is_in_list(&global_id_list, func))
        if(((Node*)ListGetElement(&global_id_list, num_of_func))->type == FUNCTION)
            return true;
        else 
            return false;
    else
        return false;
}

Func_node* find_func(char* func)
{
    for(int i = 1; i <= global_id_list.size; i ++)
    {
        Id_node* res = ListGetElement(&global_id_list, i);
        if(strncmp(res->value.text, func, strlen(func)) == 0)
            return (Func_node*)res;

    }

    return NULL;
}



void SyntaxError(const char* format, ...)
{
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
void* GetP();
Node* GetN();
char* GetV();
char* GetId();
Node* GetAssign();
Node* GetSt();
Blk_node* GetBlk();
Node* GetIf();
Func_node* GetFunc_def();
Func_node* GetFunc_call();
Id_node* GetVar_def();



const char* s = NULL;

// G ::= {Func_def | {Var_def ';'} }+ '$'
Node* GetG(const char* str)
{
    s = str;

    skipSpaces(s);

    Node* res;

    Node* val = (Node*)GetFunc_def();

    if(val == NULL)
    {
        s = str; //restoring string after wrong GetFunc_def
        val = (Node*)GetVar_def();
        skipSpaces(s);
        REQUIRE(';', s);
    }

    res = val;

    check_status(val);

    //PrintList(&Id_list);
    skipSpaces(s);

    while(*s != '$')
    {
        skipSpaces(s);

        const char* begin = s;
        Node* val2 = (Node*)GetFunc_def();

        if(val2 == NULL)
        {
            s = begin; //restoring string after wrong GetFunc_def
            val2 = (Node*)GetVar_def();
            check_status(val2);
            skipSpaces(s);
            pr_warn(LOG_CONSOLE, "yyyy\n");
            REQUIRE(';', s);
        }

        attach_node(val, right, val2);
        val = val2;
        //PrintList(&Id_list);
    }

    REQUIRE('$', s);
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
Func_node* GetFunc_call()
{
    skipSpaces(s);
    char* func_name = GetId();

    if(func_name == NULL)
        return NULL;

    if(!is_known_func(func_name))
    {
        skipSpaces(s);

        if(*s == '(')
        {
            pr_err(LOG_CONSOLE, "Undeclared function name [\"%s\"]\n",
                                func_name);
            exit(EXIT_FAILURE);
        }

        free(func_name);
        return NULL;
    }

    Func_node* called_func = find_func(func_name);

    Func_node* res = (Func_node*)create_func_call_node(func_name);
    res->id_base.alloc_name = true;

    skipSpaces(s);
    
    REQUIRE('(', s);

    skipSpaces(s);

    char* var = GetId();
    int offset = 0;
    int num_of_args = 0;


    if(var != NULL)
    {
        if(!is_known_var(var))
        {
            pr_err(LOG_CONSOLE, "Undeclared in this scope [\"%s\"]\n", var);
            exit(EXIT_FAILURE);
        }

        add_id_record(res->id_table, (id_record_t){NULL, var, offset});
        num_of_args++;

        if(num_of_args > called_func->num_of_args)
        {
            pr_err(LOG_STDERR, "wrong number of arguments:"
                               " expected: %d got: %d\n",
                               called_func->num_of_args,
                               num_of_args);
            exit(EXIT_FAILURE);
        }

        skipSpaces(s);

        while(*s == ',')
        {
            s++;
            offset += 4;
            skipSpaces(s);

            var = GetId();

            if(var == NULL)
            {
                pr_err(LOG_STDERR, "Syntax error: function call list of parameters\n");
                exit(EXIT_FAILURE);
            }

            add_id_record(res->id_table, (id_record_t){NULL, var, offset});
            num_of_args++;
            skipSpaces(s); 

            if(num_of_args > called_func->num_of_args)
            {
                pr_err(LOG_STDERR, "wrong number of arguments:"
                                   " expected: %d got: %d\n",
                                   called_func->num_of_args,
                                   num_of_args);
                exit(EXIT_FAILURE);
            }
        }
    }
    else
       skipSpaces(s); 


    skipSpaces(s)
    

    REQUIRE(')', s);


    return res;
}

// P ::= '('E')' | N | F | Var 
void* GetP()
{
    void* val = NULL;

    pr_info(LOG_CONSOLE, "Parsing Primary Expression\n\t\"%.10s...\"\n", s);

    if(*s == '(')
    {
        s++;
        skipSpaces(s);

        pr_log(LOG_CONSOLE, "Parsing Expression\n");
        val = GetE();

        check_status(val);

        skipSpaces(s);
        REQUIRE(')', s);
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

    const char* restoring = s;

    if((val = GetFunc_call()) != NULL)
    {
        pr_log(LOG_CONSOLE, "Returning from GetP();\n\t\"%.5s\"\n", s);
        return val;
    }

    s = restoring; // restoring string after wrong GetFunc_call 

    char* id = NULL;

    pr_log(LOG_CONSOLE, "Parsing Variable\n\t\"%.10s...\"\n", s);

    if((id = GetId()) != NULL)
    {
        if(is_known_var(id))
        {
            pr_log(LOG_CONSOLE, "Returning from GetP();\n\t\"%.5s\"\n", s);
            Id_node* res = (Id_node*)create_variable_node(id);
            res->alloc_name = true;
            return res;
        }
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

// Id ::= [_a-zA-Z]{[_a-zA-z0-9]}*
char* GetId()
{
    const char* begin = s;

    if(isalpha(*s) || *s == '_')
        s++;
    else
        return NULL;

    while(isalpha(*s) || *s == '_' || isdigit(*s))
        s++;

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

    if(!is_known_var(id))
    {
        pr_err(LOG_CONSOLE, "Undeclared in this scope [\"%s\"]\n", id);
        free(id);
        exit(EXIT_FAILURE);
    }

    skipSpaces(s);

    Node* expr = GetE();

    check_status(expr);
    
    Node* res = create_oper_node('=');

    attach_node(res, left, create_variable_node(id));

    ((Id_node*)res->left)->alloc_name = true;

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

    REQUIRE('(', s);

    skipSpaces(s);

    Node* condition = GetE();

    check_status(condition);

    Node* res = create_condition_node();
    attach_node(res, left, condition);

    skipSpaces(s);

    REQUIRE(')', s);

    skipSpaces(s);

    Node* body = (Node*)GetBlk();

    if(body == NULL)
    {
        body = GetSt();
        check_status(body);
    }

    attach_node(res, right, body);

    pr_log(LOG_CONSOLE, "Returning from GetIf();\n\t\"%.5s\"\n", s);
    return res;
}

// St ::= If | { Var_def | Assign | E ';'}
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

    val = (Node*)GetVar_def();

    if(val != NULL)
    {
        skipSpaces(s);
        REQUIRE(';', s);
        attach_node(res, left, val);
        pr_log(LOG_CONSOLE, "Returning from GetSt();\n\t\"%.5s\"\n", s);
        return res;
    }

    val = GetAssign();

    if(val != NULL)
    {
        skipSpaces(s);
        REQUIRE(';', s);
        attach_node(res, left, val);
        pr_log(LOG_CONSOLE, "Returning from GetSt();\n\t\"%.5s\"\n", s);
        return res;
    }

    s = begin; // restoring string after wrong GetAssignement

    val = GetE();
    if(val != NULL)
    {
        skipSpaces(s);
        REQUIRE(';', s);
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
Blk_node* GetBlk()
{
    pr_info(LOG_CONSOLE, "Parsing Block of statements\n\t\"%.10s...\"\n", s);

    if(*s != '{')
        return NULL;
    s++;
    
    Blk_node* res = create_blk_node();

    //pr_warn(LOG_CONSOLE, "PrintList(&id_table_list)\n");
    //PrintList(&id_table_list);

    ListInsertBack(&id_table_list, res->id_table);

    //pr_warn(LOG_CONSOLE, "PrintList(&id_table_list)\n");
    //PrintList(&id_table_list);

    Node* val1 = GetSt();
    attach_node(res, left, val1);

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

    pr_log(LOG_CONSOLE, "Returning from GetBlk();\n\t\"%.5s\"\n", s);

    //pr_warn(LOG_CONSOLE, "PrintList(&id_table_list)\n");
    //PrintList(&id_table_list);

    ListPopBack(&id_table_list);

    //pr_warn(LOG_CONSOLE, "PrintList(&id_table_list)\n");
    //PrintList(&id_table_list);

    return res;
}

// Func_def ::= Id'(' Id{',' V}* ')' Blk
Func_node* GetFunc_def()
{
    char* id =  GetId();

    if(id == NULL)
        return NULL;

    pr_info(LOG_CONSOLE, "new function name \"%s\"\n", id);

    skipSpaces(s);

    if(*s != '(')
    {
        free(id);
        return NULL;
    }

    s++;
    
    Func_node* res = create_func_node(id);
    res->id_base.alloc_name = true;

    int offset = 0;
    int num_of_args = 0;


    skipSpaces(s);

    char* var = GetId();

    skipSpaces(s);

    if(var != NULL)
    {
        pr_log(LOG_CONSOLE, "adding new id_record with \"%s\" name\n", var);

        add_id_record(res->id_table, (id_record_t){NULL, var, offset});
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
            add_id_record(res->id_table, (id_record_t){NULL, var, offset});
            num_of_args++;
            skipSpaces(s);
        }
    }
    else
        skipSpaces(s);

    
    REQUIRE(')', s);

    res->num_of_args = num_of_args;

    //pr_warn(LOG_CONSOLE, "Inserted prop of func \"%s\""
    //                     " [name = %s, num of args = %d]\n",
    //                     id, id, num_of_args);

    //pr_warn(LOG_CONSOLE, "PrintList(&id_table_list)\n");
    //PrintList(&id_table_list);

    ListInsertBack(&id_table_list, res->id_table);
    
    //current_id_table = res->id_table;

    skipSpaces(s);

    //pr_warn(LOG_CONSOLE, "PrintList(&id_table_list)\n");
    //PrintList(&id_table_list);

    Node* func_body = (Node*)GetBlk();

    if(func_body == NULL)
    {
        pr_err(LOG_STDERR, "Syntax error: function definiton [func:\"%s\"]\n",
                           res->id_base.value.text);
        exit(EXIT_FAILURE);
    }

    attach_node(res, left, func_body);

    ListInsertBack(&global_id_list, res);

    ListPopBack(&id_table_list);

    //current_id_table = NULL;

    return res;
}

// Var_def ::= "let" Id {'='E'}?
Id_node* GetVar_def()
{
    pr_info(LOG_CONSOLE, "Parsing Var definition\n\t\"%.10s...\"\n", s);

    skipSpaces(s);

    if(strncmp(s, "let", sizeof("let") - 1) != 0)
        return NULL;

    s += sizeof("let") - 1; // skipping "let" word
    //skipAlphas(s);
    skipSpaces(s);

    char* id = GetId(); 

    check_status(id);

    Id_node* var = create_var_def_node(id);
    var->alloc_name = true;

    if(id_table_list.size == 0)
    {
        pr_log(LOG_CONSOLE, "Adding a new global variable\n");
        ListInsertBack(&global_id_list, var);
    }
    else
    {
        id_table_t* last_table = ListGetElement(&id_table_list, id_table_list.size);
        id_record_t* last_record = get_last_record(last_table);

        //PrintList(&id_table_list);
        //print_id_table(last_table);

        if(last_record == NULL)
            add_id_record(last_table, (id_record_t){NULL, id, 4});
        else
            add_id_record(last_table, (id_record_t){NULL, id, last_record->offset + 4});
    }

    skipSpaces(s);
    if(*s == '=')
    {
       s++;
       skipSpaces(s);
    
       Node* num = GetE();
       check_status(num);
       attach_node(var, left, num);
    }

    skipSpaces(s);

    pr_info(LOG_CONSOLE, "Returning from GetVar_def()\n\t\"%.10s...\"\n", s);

    return var;
}

int main(int argc, char **argv)
{
    ListCtor(&global_id_list, 20);
    ListCtor(&id_table_list, 20);

    if(argc < 2)
    {
        pr_err(LOG_STDERR, "no input\n");
        exit(1);
    }

    Tree* tree = tree_init();

    int sz = fileSize(argv[1]);
    char* txt = readText(argv[1], sz);

    tree->root = GetG(txt);
    dump_tree_dot("dot_out", tree);

    tree_free(&tree);
    free(txt);

    ListDtor(&id_table_list);
    ListDtor(&global_id_list);

    return 0;
}
