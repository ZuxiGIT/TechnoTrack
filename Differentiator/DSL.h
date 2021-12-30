#ifndef DSL_H
#define DSL_H

#include "./lib/tree/tree.h"

#define add_node(tree, node, side, type, value)\
{\
    node->side = create_node(type, (value_t)value);\
    node->side->parent = node;\
    tree->size++;\
}
#endif /* DSL_H */
