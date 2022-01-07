#ifndef DSL_H
#define DSL_H

#include <stdlib.h>
#include "./lib/tree/tree.h"

/*
 * side means left child or right
 * ___
 * |
 * | side = left
 * | side = right
 * |__
 */

#define attach_node(node, side, child)\
{\
    node->side = child;\
    node->side->parent = node;\
}

#define add_node(tree, node, side, type, value)\
{\
    node->side = create_node(type, (value_t)value);\
    node->side->parent = node;\
    if(tree != NULL)\
        tree->size++;\
}

#define _2_str(name) #name
#define create_undefined_node() create_node(UNDEFINED, (value_t)((char*)NULL))
#define create_const_node(num)  create_node(CONST,     (value_t)((double)num))
#define create_oper_node(oper)  create_node(OPER,      (value_t)((double)oper))
#define create_func_node(func)  create_node(FUNC,      (value_t)func)
#define create_empty_node()     create_node(EMPTY,     (value_t)((char*)NULL))

// finally i faced the situation where construction do{...}while(0) is necessary
// its better to rewrite this func in tree.c as it is more complicated as usual macro
#define replace_node(_old_node, _new_node)\
do{\
    Node* old_node = _old_node;\
    Node* new_node = _new_node;\
    if(node->parent == NULL)\
    {\
        node_free(node->left);\
        node_free(node->right);\
        if(node->alloc)\
            free(node->value.text);\
        memcpy(old_node, new_node, sizeof(Node));\
        /*free(_new_node) does not work? research is needed */\
        free(new_node);\
    }\
    else\
    {\
        Node* old_addr = old_node;\
        Node* parent = old_node->parent;\
        node_free(old_node);\
        if(parent->left == old_addr)\
            parent->left = new_node;\
        if(parent->right == old_addr)\
            parent->right = new_node;\
        new_node->parent = parent;\
    }\
}while(0)

#endif /* DSL_H */
