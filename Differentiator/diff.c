#include <assert.h>
#include "diff.h"
#include "DSL.h"
#include "./lib/logger/logger.h"

static Node* differentiate_node(Node* node)
{
    if(node->type == CONST)
        return create_const_node(0); 

    if(node->type == VAR)
        return create_const_node(1);

    if(node->type == EMPTY)
        return node;

    if(node->type == OPER)

    if(node->type == FUNC)
    {
        #define func(name, diff)\
        if(strncasecmp(node->value.text, #name, sizeof(#name) - 1) == 0)\
        {\
            diff\
        }

        #include "./lib/tree/func"

        #undef func
    }

    pr_err(LOG_CONSOLE, "Undefined node[type: %d]\n", node->type);

}

Tree* differentiate_tree(const Tree* tree)
{
    if(tree == NULL)
        return NULL;
    Tree* res = tree_init();

    res->root = differentiate_node(tree->root);

    return res;
}
