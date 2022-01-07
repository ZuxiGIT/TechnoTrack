#include <stdio.h>
#include <stdlib.h>

#include "DSL.h"
#include "diff.h"
#include "./lib/TextLib/File.h"
#include "./lib/logger/logger.h"
//#include "./lib/tree/tree.h"

int main()
{
    Tree* tree = tree_init();
    //parse_tree_from_source(argv[1], tree);
    //tree->root->type = OPER;
    //tree->root->value.text = "+";
    tree->root = create_node(FUNC, (value_t)"sin");
    tree->size++; // user requirement to increment the size of a tree
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

    printf("Ok!\n");
    Tree* tree3 = parse_tree_from_source("../input1"); /* probably 
    should be moved to unit tests section */
    // Also, do diff's functions exist inside tree library?
    printf("not Ok!\n");
    if(tree3 == NULL)
        printf("Ok!\n");
    printf("saving tree3 in dot\n");
    dump_tree_dot("dot_test3", tree3);
    printf("saving tree3 in tex\n");
    dump_tree_tex("tex_test3", tree3);
    printf("saving tree3\n");
    save_tree("test3.tr", tree3);
    
    Tree* tree5 = tree_init();
    if(tree3 != NULL)
        tree5->root = copy_subtree(tree3->root);
    //printf("got addres %p\n", tree5->root);
    
    if(tree3 != NULL)
        printf("tree3 size %d\n", tree3->size);


    Tree* tree2 = load_tree("test3.tr");
    printf("saving tree2 in dot\n");
    dump_tree_dot("dot_test2", tree2);
    save_tree("test2.tr", tree2);

    Tree* tree4 = differentiate_tree(tree2);
    printf("saving tree4\n");
    save_tree("test4.tr", tree4);
    printf("saving tree4 in dot\n");
    dump_tree_dot("dot_test4", tree4);
    printf("saving tree4 in tex\n");
    dump_tree_tex("tex_test4", tree4);

    printf("saving tree5\n");
    save_tree("test5.tr", tree5);

    printf("saving tree2\n");
    save_tree("test2", tree2);
    //dump_tree_dot("dot_test2", tree2);
    //dump_tree_tex("tex_test2", tree2);
    
    printf("tree->size = %d\n", tree->size);
    printf("sizeof(tree->root->type) = %lu\n", sizeof(tree->root->type));
    save_tree("test", tree);

    printf("free tree\n");
    tree_free(&tree);
    printf("free tree2\n");
    tree_free(&tree2);
    printf("free tree3\n");
    tree_free(&tree3);
    printf("free tree4\n");
    tree_free(&tree4);
    printf("free tree5\n");
    tree_free(&tree5);

    return 0;
}
