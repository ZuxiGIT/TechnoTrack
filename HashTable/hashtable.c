#include "hashtable.h"

#include <string.h>
#include <stdlib.h>

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
    hashtable_t* htab = (hashtable_t*)calloc(1, sizeof(hashtable_t));

    htab->buckets = (list_t*)calloc(bucket_count, sizeof(list_t));
    htab->size = bucket_count;

    for(int i = 0; i < bucket_count; i++)
    {
        ListCtor(htab->buckets + i, 10);
        htab->buckets[i].resizeable = true;
    }

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
    hash_t hash = hashFunc((void*)key, strlen(key)) % htab->size;

    return ListInsertBack(htab->buckets + hash, key, val) > 0 ? 0 : -1;
}

int HashTableGetElemByKey(hashtable_t* htab, const char* key)
{
    hash_t hash = hashFunc((void*) key, strlen(key)) % htab->size;

    return ListGetElemByKey(htab->buckets + hash, key);
}


//----------------------------------hash functions------------------------------

hash_t simpleHash(void* data, size_t data_len)
{
    hash_t hash = 5381;
    const unsigned char* c = data;

    while (data_len-- > 0)
        hash = ((hash << 5) + hash) + *c++; /* hash * 33 + c */

    return hash;
}
