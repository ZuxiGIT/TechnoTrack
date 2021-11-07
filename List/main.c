#include <stdio.h>
#include "list.h"
#include <string.h>
#include <stdlib.h>

int main()
{
    List lst = {};
    ListCtor(&lst, 10);
    printf("The cell address is %d\n", ListInsertFront(&lst, 15));
    LogList("before1", &lst);
    printf("The cell address is %d\n", ListInsertBack(&lst, 25));
    LogList("before2", &lst);
    printf("Deletind %d with %d data\n", 2, ListDelete(&lst, 2));
    LogList("after1", &lst);
    printf("The cell address is %d\n", ListInsertBack(&lst, 35));
    LogList("after2", &lst);
    //printf("The cell address is %d\n", ListInsertAfter(&lst, 1, 45));
   
    printf("%d----------\n", BUFSIZ);
    PrintList(&lst);
    ListDtor(&lst);
    return 0;
}
