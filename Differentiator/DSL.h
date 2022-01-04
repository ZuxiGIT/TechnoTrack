#ifndef DSL_H
#define DSL_H

#include "./lib/tree/tree.h"

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

#endif /* DSL_H */
