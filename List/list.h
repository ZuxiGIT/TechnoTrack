#ifndef LIST_H
#define LIST_H

typedef struct
{
    int data;
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
int ListInsertBack(List* list, int num);
int ListInsertFront(List* list, int num);
int ListInsertAfter(List* list, int location, int num);
int ListInsertBefore(List* list, int location, int num);
int ListDelete(List* list, int location);
int ListPopBack(List* list);
int ListPopFront(List* list);
int ListDeleteAfter(List* list, int location);
int ListDeleteBefore(List* list, int location);
void PrintList(List* list);
void LogList(const char* pathname, List* list);

#endif /* LIST_H */
