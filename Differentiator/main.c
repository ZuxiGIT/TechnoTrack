#include <stdio.h>

#include "DSL.h"
#include "diff.h"
#include "./lib/logger/logger.h"

int main()
{
    Tree* tree = tree_init();
    //parse_tree_from_source(argv[1], tree);
    //tree->root->type = OPER;
    //tree->root->value.text = "+";
    tree->root = create_node(FUNC, (value_t) "sin");
    tree->size++;
    //printf("\"+\" ACII is %s\n", ((value_t) "+").text);
    //printf("\"+\" ACII is %lf\n", tree->root->value.num);

    add_node(tree, tree->root, left, CONST, 10.);
    add_node(tree, tree->root, right, CONST, 20.);

    //tree->root->left = CREATE_NODE(CONST, (value_t) 10.);
    //tree->root->left->parent = tree->root;
    //tree->root->right = CREATE_NODE(CONST, (value_t) 20.);
    //tree->root->right->parent = tree->root;
    //freopen(NULL, "w", stdout);
    //pr_err(LOG_STDERR, "test\n");
    //freopen(NULL, "w", stdout);
    printf("sizeof(\"type\") = %lu\n", sizeof("type"));

    Tree* tree3 = parse_tree_from_source("../input");
    printf("tree3 size %d\n", tree3->size);
    dump_tree_tex("tex_test3", tree3);
    dump_tree_dot("test3", tree3);
    save_tree("test3.tr", tree3);

    Tree* tree2 = load_tree("test3.tr");
    Tree* tree4 = differentiate_tree(tree2);


    save_tree("test2", tree2);
    //dump_tree_dot("dot_test2", tree2);
    //dump_tree_tex("tex_test2", tree2);
    
    printf("tree->size = %d\n", tree->size);
    printf("sizeof(tree->root->type) = %lu\n", sizeof(tree->root->type));
    save_tree("test", tree);

    tree_free(&tree);
    tree_free(&tree2);
    tree_free(&tree3);
    tree_free(&tree4);
    return 0;
}
