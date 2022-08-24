#include <stdio.h>

#include "hashtable.h"

int main()
{
    hashtable_t* htab = HashTableCtor(32);
    
    HashTableAddElem(htab, "example", 42);
    printf("val: %d", HashTableGetElemByKey(htab, "example"));

    HashTableDtor(htab);

    return 0;
}
