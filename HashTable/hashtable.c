#include "hashtable.h"
#include "./lib/logger/logger.h"

#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct hashtable
{
    list_t* buckets;
    int size;
};

typedef hash_t (* hashfunc_t ) (void* data, size_t data_len);

hash_t simpleHash(void* data, size_t data_len);

static hashfunc_t hashFunc = simpleHash;



hashtable_t* HashTableCtor(int bucket_count)
{
    if(__builtin_popcount(bucket_count) != 1)
        return NULL;

    hashtable_t* htab = (hashtable_t*)calloc(1, sizeof(hashtable_t));

    if(!htab)
        return NULL;

    htab->buckets = (list_t*)calloc(bucket_count, sizeof(list_t));
    htab->size = bucket_count;

    return htab;
}

void HashTableDtor(hashtable_t* htab)
{
    for(int i = 0; i < htab->size; i ++)
        ListDtor(htab->buckets + i);

    free(htab->buckets);
    free(htab);
}

int HashTableAddElem(hashtable_t* htab, const char* key, int val)
{
    hash_t hash = hashFunc((void*)key, strlen(key)) & (htab->size - 1);

    if((htab->buckets[hash].cells) == NULL)
    {
        ListCtor(htab->buckets + hash, 10);
        htab->buckets[hash].resizeable = true;
    }

    return ListInsertBack(htab->buckets + hash, key, val) > 0 ? 0 : -1;
}

int HashTableGetElemByKey(hashtable_t* htab, const char* key)
{
    hash_t hash = hashFunc((void*) key, strlen(key)) & (htab->size - 1);

    if((htab->buckets[hash].cells) == NULL)
        return INT_MAX;

    return ListGetElemByKey(htab->buckets + hash, key);
}

int HashTableSizeInBytes(hashtable_t* htab)
{
    int size = sizeof(hashtable_t);

    for(int i = 0; i < htab->size; i ++)
        if(htab->buckets[i].cells != NULL)
        {
            size += sizeof(list_t);
            size += sizeof(cell_t) * htab->buckets[i].size;
        }

    return size;
}

//----------------------------------hash functions------------------------------
#define likely(x)         __builtin_expect(!!(x),1)
#define unlikely(x)       __builtin_expect(!!(x),0)

hash_t simpleHash(void* data, size_t data_len)
{
    hash_t hash = 5381;
    const unsigned char* c = data;

    while (likely(data_len-- > 0))
        hash = ((hash << 5) + hash) + *c++; /* hash * 33 + c */

    return hash;
}
#undef unlikely 
#undef likely
