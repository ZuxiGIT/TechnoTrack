#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

// double linked list

typedef struct
{
    const char* key;
    int value;
    int next;
    int prev;
} cell_t;

typedef struct
{
    cell_t* cells;
    int size;
    int capacity;
    int free;
    int head;
    int tail;
    bool resizeable;
} list_t;

void ListCtor(list_t* list, int size);
void ListDtor(list_t* list);
int ListInsertBack(list_t* list, const char* key, int val);
int ListInsertFront(list_t* list, const char* key, int val);
int ListInsertAfter(list_t* list, int location, const char* key, int val);
int ListInsertBefore(list_t* list, int location, const char* key, int val);
int ListDelete(list_t* list, int location);
int ListPopBack(list_t* list);
int ListPopFront(list_t* list);
int ListDeleteAfter(list_t* list, int location);
int ListDeleteBefore(list_t* list, int location);
void PrintList(list_t* list);
void LogList(const char* pathname, list_t* list);
void ListResize(list_t* list, int new_size);
int ListGetElemByKey(list_t* list, const char* key);

#endif /* LIST_H */
