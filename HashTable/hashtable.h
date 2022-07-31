#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "./lib/list/list.h"

typedef unsigned long long hash_t;

typedef struct
{
    list_t* buckets;
    int size;
} hashtable_t;

void HashTableCtor(hashetable_t* htab, int bucket_count);
void HashTableDtor(hashtable_t* htab);
int  HashTableAddElem(hashtable_t* htab, void* data, 
                     hash_t (*findHash) (void* data, size_t data_len));

#endif /* HASHTABLE_H */
