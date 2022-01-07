#ifndef DIFF_H
#define DIFF_H
#include "./lib/tree/tree.h"

Tree* differentiate_tree(const Tree* tree);
void optimize_subtree(Node* node);
void optimize_tree(Tree* tree);

#endif /* DIFF_H */
