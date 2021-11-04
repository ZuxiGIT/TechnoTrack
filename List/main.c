#include <stdio.h>
#include "list.h"
#include <string.h>
#include <stdlib.h>

int main()
{
    List lst = {};
    ListCtor(&lst, 10);
    printf("The cell address is %d\n", ListInsertBack(&lst, 5));
    printf("The cell address is %d\n", ListInsertBack(&lst, 5));
    
    PrintList(&lst);
    ListDtor(&lst);
    return 0;
}
