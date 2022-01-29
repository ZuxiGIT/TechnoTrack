#ifndef LIST_H
#define LIST_H

typedef struct
{
    void* data;
    int next;
    int prev;
} Cell;

typedef struct
{
    Cell* cells;
    int size;
    int capacity;
    int free;
    int head;
    int tail;
} List;

void ListCtor(List* list, int size);
void ListDtor(List* list);
int ListInsertBack(List* list, void* num);
int ListInsertFront(List* list, void* num);
int ListInsertAfter(List* list, int location, void* num);
int ListInsertBefore(List* list, int location, void* num);
void* ListDelete(List* list, int location);
void* ListPopBack(List* list);
void* ListPopFront(List* list);
void* ListDeleteAfter(List* list, int location);
void* ListDeleteBefore(List* list, int location);
void PrintList(List* list);
void LogList(const char* pathname, List* list);

#endif /* LIST_H */
