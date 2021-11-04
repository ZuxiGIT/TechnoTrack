#ifndef LIST_H
#define LIST_H


typedef struct
{
    int* data;
    int* next;
    int capacity;
    int head;
    int tail;
} List;

void ListCtor(List* list, int size);
void ListDtor(List* list);
int ListInsertBack(List* list, int num);
int ListInsertAfter(List* list, int location, int num);
void PrintList(List* list);
#endif /* LIST_H */
