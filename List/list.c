#include "list.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./logger/logger.h"

int _findEmptyCell(List* list)
{
    int i = 0;

    while((list->next[i] != -1) && ( i < list->capacity)) i++;
    printf("_findEmptyCell has i = %d\n", i);
    if(list->next[i] != -1)
        return -1;
    
    return i;
}

void ListCtor(List* list, int size)
{
    list->data = (int*)calloc(size, sizeof(int));
    list->next = (int*)calloc(size, sizeof(int));

    memset(list->next, -1, 4 * size);

    *list->next = 0;

    list->capacity = size;

    list->head = list->tail = 0;
}

void ListDtor(List* list)
{
    free(list->data);
    free(list->next);
}

int ListInsertBack(List* list, int num)
{
    if(!list->head)
    {
        list->data[1] = num;
        list->head = list->tail = 1;
        list->next[1] = 0;
        return 1; 
    }
    
    printf("searching new cell\n");

    int found = _findEmptyCell(list);

    if(found == -1)
    {
        printf("Found empty cell %d\n", found);

        list->data[found] = num;
        list->next[list->tail] = found;
        list->next[found] = 0;
        list->tail = found;
    }
    else
        pr_err(LOG_CONSOLE, "List is full\n");

    return found;
}

int ListInsertAfter(List* list, int location, int num)
{
    
} 

void PrintList(List* list)
{
    printf("list.head = %d\tlist.tail = %d\n", list->head, list->tail);
    for(int i = 0; i < list->capacity; i ++)
    {
        printf("list.data[%d] = %d\tlist.next[%d] = %d\n", i, list->data[i], i,  list->next[i]);
    }
}
