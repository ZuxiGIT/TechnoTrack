#include "tree.h"
#include <stdlib.h>

Tree treeInit()
{
    Tree res = {}; 

    res.nodes = (Node*)calloc(1, sizeof(Node));
    res.size = 1;

    res.nodes->str = "It is unknown?";
}
