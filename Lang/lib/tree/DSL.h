#ifndef DSL_H
#define DSL_H

#include <stdlib.h>
#include "_tree.h"

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
    ((Node*)node)->side = (Node*)child;\
    ((Node*)node)->side->parent = (Node*)node;\
}

#define add_node(tree, node, side, type, value)\
{\
    node->side = create_node(type, (value_t)value);\
    node->side->parent = node;\
    if(tree != NULL)\
        tree->size++;\
}

#define _2_str(name) #name
#define create_undefined_node()  create_node(UNDEFINED, (value_t)((char*)NULL))
#define create_const_node(num)   create_node(CONSTANT,  (value_t)((double)num))
#define create_oper_node(oper)   create_node(OPERATOR,  (value_t)((double)oper))
#define create_func_node(func)   create_node(FUNCTION,  (value_t)func)
#define create_condition_node()  create_node(CONDITION, (value_t)((char*)NULL))
#define create_variable_node(var)create_node(VARIABLE,  (value_t)var)
#define create_func_call_node(func)  create_node(FUNC_CALL, (value_t)func)
#define create_semicolon_node()  create_node(SEMICOLON, (value_t)((double)';'))
#define create_empty_node()      create_node(EMPTY,     (value_t)((char*)NULL))
#define create_blk_node()        create_node(BLOCK_OF_STATEMENTS,\
                                            (value_t)((char*)NULL))
#define create_var_def_node(var)    create_node(VARIABLE_DEF, (value_t)var)


#endif /* DSL_H */
