#include "hashtable.h"

#include <string.h>

void HashTableCtor(hashetable_t* htab, int bucket_count)
{
    htab->buckets = (list_t*)calloc(bucket_count, sizeof(list_t));
    htab->size = bucket_count;

    for(int i = 0; i < bucket_count; i++)
    {
        ListCtor(htab->bucket + i, 10);
        htab->buckets[i].resizeable = true;
    }
}

void HashTableDtor(hashtable_t* htab)
{
    for(int i = 0; i < htab->size; i ++)
        ListDtor(htab->buckets + i);

    free(htab->buckets);
}

int HashTableAddElem(hashtable_t* htab, void* data, 
                     hash_t (*findHash) (void* data, size_t data_len))
{
    hash_t hash = findHash(data, strlen(data));
    hash %= htab->size;

    return ListInsertBack(htab->buckets[hash], data) > 0 ? 0 : -1;
}
