#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

// double linked list

typedef struct
{
    void* data;
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
int ListInsertBack(list_t* list, void* data);
int ListInsertFront(list_t* list, void* data);
int ListInsertAfter(list_t* list, int location, void* data);
int ListInsertBefore(list_t* list, int location, void* data);
int ListDelete(list_t* list, int location);
int ListPopBack(list_t* list);
int ListPopFront(list_t* list);
int ListDeleteAfter(list_t* list, int location);
int ListDeleteBefore(list_t* list, int location);
void PrintList(list_t* list);
void LogList(const char* pathname, list_t* list);
void ListResize(list_t* list, int new_size);

#endif /* LIST_H */
