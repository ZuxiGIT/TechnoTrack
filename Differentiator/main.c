#include <stdio.h>

#include "./lib/tree/tree.h"

int main()
{
    Tree* tree = tree_init();
    //parse_tree_from_source(argv[1], tree);
    //tree->root->type = OPER;
    //tree->root->value.text = "+";
    tree->root = create_node(OPER, (value_t) "+");

    add_node(tree, tree->root, left, CONST, 10.);
    add_node(tree, tree->root, right, CONST, 20.);

    //tree->root->left = CREATE_NODE(CONST, (value_t) 10.);
    //tree->root->left->parent = tree->root;
    //tree->root->right = CREATE_NODE(CONST, (value_t) 20.);
    //tree->root->right->parent = tree->root;
    printf("tree->size = %d\n", tree->size);
    printf("sizeof(\"type\") = %lu\n", sizeof("te"));
    printf("sizeof(tree->root->type) = %lu\n", sizeof(tree->root->type));
    save_tree("test", tree);

    tree_free(&tree);
    return 0;
}
