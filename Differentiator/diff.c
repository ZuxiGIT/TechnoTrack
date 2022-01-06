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
        return copy_node(node);

    if(node->type == OPER)
    {
        if(node->value.num == '+')
        {
            Node* res = create_oper_node('+');
            attach_node(res, left, differentiate_node(node->left));
            attach_node(res, right, differentiate_node(node->right));
            return res;
        }
        else if(node->value.num == '-')
        {
            Node* res = create_oper_node('-');
            attach_node(res, left, differentiate_node(node->left));
            attach_node(res, right, differentiate_node(node->right));
            return res;
        }
        else if(node->value.num == '*')
        {
            Node* res = create_oper_node('+');

            Node* left_copy = copy_subtree(node->left);
            Node* left_diff = differentiate_node(node->left);

            Node* right_copy = copy_subtree(node->right);
            Node* right_diff = differentiate_node(node->right);

            attach_node(res, left, create_oper_node('*'));

            Node* res_left = res->left;

            attach_node(res_left, left, left_diff);
            attach_node(res_left, right, right_copy);

            attach_node(res, right, create_oper_node('*'));
            
            Node* res_right = res->right;

            attach_node(res_right, left, left_copy);
            attach_node(res_right, right, right_diff);

            return res;
        }
        else if(node->value.num == '/')
        {
            Node* res = create_oper_node('/');
            
            Node* left_copy = copy_subtree(node->left);
            Node* left_diff = differentiate_node(node->left);

            Node* right_copy_num = copy_subtree(node->right);
            Node* right_copy_denom = copy_subtree(node->right);
            Node* right_diff = differentiate_node(node->right);

            Node* numerator = create_oper_node('-');

            attach_node(numerator, left, create_oper_node('*'));

            Node* num_left = numerator->left;

            attach_node(num_left, left, left_diff);
            attach_node(num_left, right, right_copy_num);

            attach_node(numerator, right, create_oper_node('*'));

            Node* num_right = numerator->right;

            attach_node(num_right, left, left_copy);
            attach_node(num_right, right, right_diff);

            Node* denominator = create_func_node("^");

            attach_node(denominator, left, right_copy_denom);
            attach_node(denominator, right, create_const_node(2));

            attach_node(res, left, numerator);
            attach_node(res, right, denominator);

            return res;
        }
    }

    if(node->type == FUNC)
    {
        #define func(name, diff, latex_begin, latex_mid, latex_end)\
        if(strncasecmp(node->value.text, #name, sizeof(#name) - 1) == 0)\
        {\
            diff\
        }

        #include "./lib/tree/func"

        #undef func
    }

    pr_err(LOG_CONSOLE, "Undefined node[type: %d]\n", node->type);
    return NULL;
}

Tree* differentiate_tree(const Tree* tree)
{
    if(tree == NULL)
        return NULL;

    Tree* res = tree_init();

    res->root = differentiate_node(tree->root);

    return res;
}

void optimize_subtree(Node* tree)
{
    assert(NULL && "Not implemented yet");
}

void optimize_tree(Tree* tree)
{
    assert(NULL && "Not implemented yet");
}
