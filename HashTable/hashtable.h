#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include "./lib/list/list.h"


typedef unsigned long long hash_t;

//struct hashtable;
typedef struct hashtable hashtable_t;

//enum
//{
//} hashFunctions;

hashtable_t* HashTableCtor(int bucket_count);
void HashTableDtor(hashtable_t* htab);
int HashTableAddElem(hashtable_t* htab, const char* key, int val); 
int HashTableGetElemByKey(hashtable_t* htab, const char* key);

#endif /* HASHTABLE_H */
