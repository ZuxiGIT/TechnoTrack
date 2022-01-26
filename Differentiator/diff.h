#ifndef DIFF_H
#define DIFF_H
#include "./lib/tree/tree.h"

Tree* differentiate_tree(const Tree* tree);
void optimize_subtree(Node* node);
void optimize_tree(Tree* tree);
int print_random_str(char buff, int max);

#endif /* DIFF_H */
