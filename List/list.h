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
int ListInsertAfter(List* list, int location, int num);
void PrintList(List* list);
void LogList(const char* pathname, List* list);

#endif /* LIST_H */
