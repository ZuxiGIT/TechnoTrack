#include <stdio.h>

#include "./lib/tree/tree.h"
#include "./lib/logger/logger.h"

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
    freopen(NULL, "w", stdout);
    pr_err(LOG_STDERR, "test\n");
    freopen(NULL, "w", stdout);
    Tree* tree2 = load_tree("test");
    save_tree("test2", tree2);
    
    printf("tree->size = %d\n", tree->size);
    printf("sizeof(\"type\") = %lu\n", sizeof("te"));
    printf("sizeof(tree->root->type) = %lu\n", sizeof(tree->root->type));
    save_tree("test", tree);

    tree_free(&tree);
    return 0;
}
