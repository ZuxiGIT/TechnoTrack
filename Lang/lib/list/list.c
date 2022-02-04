#include "list.h"
#include "../logger/logger.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define VERIFY_CELL(loc, ret)\
    if(list->cells[loc].prev == -1)\
    {\
        pr_err(LOG_CONSOLE, "Bad location (cell with %d address does not exist\n",\
                loc);\
        return ret;\
    }\
\
    if(!loc)\
    {\
        pr_err(LOG_CONSOLE, "Bad location (location can not be equal 0)\n");\
        return ret;\
    }\

static int _findEmptyCell(List* list)
{

    int res = list->free;
    if(res != 0)
        list->free = list->cells[list->free].next;
        
    return res;
}

void ListCtor(List* list, int capacity)
{
    list->cells = (Cell*)calloc(capacity, sizeof(Cell));

    //*(list->cells).next = *(list->cells).prev = *(list->cells).data = 0;
    memset(list->cells, 0, sizeof(Cell));

    for(int i = 1; i < capacity - 1; i++)
    {
        list->cells[i].next = i + 1;
        list->cells[i].prev = -1;
    }

    list->cells[capacity - 1].next = 0;
    list->cells[capacity - 1].prev = -1;

    list->size = 0;
    list->capacity = capacity;

    list->head = list->tail = 0;
    list->free = 1;
}

void ListDtor(List* list)
{
    free(list->cells);
}

int ListInsertBack(List* list, void* num)
{
    if(!list->head)
    {
        list->cells[1].data = num;
        list->head = list->tail = 1;
        list->cells[1].next = 0;
        list->cells[1].prev = 0;
        list->free = 2;
        list->size++;
        return 1; 
    }
    
    return ListInsertAfter(list, list->tail, num);
}

int ListInsertFront(List* list, void* num)
{
    if(!list->head)
    {
        list->cells[1].data = num;
        list->head = list->tail = 1;
        list->cells[1].next = 0;
        list->cells[1].prev = 0;
        list->free = 2;
        list->size++;
        return 1; 
    }

    return ListInsertBefore(list, list->head, num);
}

int ListInsertAfter(List* list, int location, void* num)
{
    VERIFY_CELL(location, -1);

    int found = _findEmptyCell(list);

    printf("found empty cell with %d address (location = %d)"
            " to insert value = %p\n",
            found, location, num);

    if(found)
    {
        list->cells[found].data = num;

        list->cells[found].prev = location;

        list->cells[found].next = list->cells[location].next;

        if(list->cells[location].next)
            list->cells[list->cells[location].next].prev = found;

        list->cells[location].next = found;

        if(list->tail == location)
            list->tail = found;

        list->size++;
    }
    else
        pr_err(LOG_CONSOLE, "List is full\n");
    
    return found;
} 

int ListInsertBefore(List* list, int location, void* num)
{
    VERIFY_CELL(location, -1);

    int found = _findEmptyCell(list);

    if(found)
    {
        list->cells[found].data = num;
        list->cells[found].next = location;
        list->cells[found].prev = list->cells[location].prev;

        if(list->cells[location].prev)
            list->cells[list->cells[location].prev].next = found;

        list->cells[location].prev = found;

        if(list->head == location)
            list->head = found;

        list->size++;
    }
    else
        pr_err(LOG_CONSOLE, "List is full\n");

    return found;
}

void* ListDelete(List* list, int location)
{
    VERIFY_CELL(location, NULL);

    if(list->tail == location)
        list->tail = list->cells[location].prev;
    if(list->head == location)
        list->head = list->cells[location].next;

    if(list->cells[location].next)
    {
        if(list->cells[location].prev)
        {
            list->cells[list->cells[location].prev].next = list->cells[location].next;
            list->cells[list->cells[location].next].prev = list->cells[location].prev;
        }
        else
            list->cells[list->cells[location].next].prev = 0;
    }
    else
    {
        if(list->cells[location].prev)
            list->cells[list->cells[location].prev].next = 0;
    }

    list->cells[location].next = list->free;
    list->free = location;

    list->cells[location].prev = -1;

    list->size--;
    
    return list->cells[location].data;
}

void* ListPopFront(List* list)
{
    return ListDelete(list, list->head);
}

void* ListPopBack(List* list)
{
    return ListDelete(list, list->tail);
}

void* ListDeleteAfter(List* list, int location)
{
    VERIFY_CELL(location, NULL);
    
    if(location == list->tail)
    {
        pr_err(LOG_CONSOLE, "Bad location (cell after cell with address %d does not exist\n", location);
        return NULL;
    }

    return ListDelete(list, list->cells[location].next);
}

void* ListDeleteBefore(List* list, int location)
{
    VERIFY_CELL(location, NULL);

    if(location == list->head)
    {
        pr_err(LOG_CONSOLE, "Bad location (cell before cell with address %d does not exist\n", location);
        return NULL;
        
    }

    return ListDelete(list, list->cells[location].prev);
}

void PrintList(List* list)
{
    printf("list.head = %d\tlist.tail = %d\tlist.free = %d\n"
           "list.size = %d\tlist.capacity = %d\n",
            list->head, list->tail, list->free,
            list->size, list->capacity);

    for(int i = 0; i <= list->size; i ++)
    {
        printf("Cell[%d]: data = 0x%p\tnext = %d\tprev = %d\n",
                i, list->cells[i].data,
                list->cells[i].next, 
                list->cells[i].prev);
    }
}

static int _shift(char* buff, int shift)
{
    for(int i = 0; i < shift; i++)
        sprintf(buff + i, "\t");

    return shift;
}

void LogList(const char* pathname, List* list)
{
    static char log_buffer[4096] = {};
    static int buff_pos = 0;
    static int shift = 0;

    buff_pos += sprintf(log_buffer, "digraph list{\n\trankdir=HR;\n");
    shift++;

    buff_pos += _shift(log_buffer + buff_pos, shift);

    buff_pos += sprintf(log_buffer + buff_pos, "list_info[shape=record, "
                        "label = \"list.head\\n%d | list.tail\\n%d |"
                        " list.free\\n%d\"];\n",
                        list->head, list->tail, list->free);



    for(int i = 1; i < list->capacity; i++)
    {
        buff_pos += _shift(log_buffer + buff_pos, shift);
        buff_pos += sprintf(log_buffer + buff_pos, "cell_%d [shape=record, "
                            "label=\"{ addres\\n%d | data\\n%s(0x%p) }| "
                            "<next> next\\n%d | <prev> prev\\n%d \"];\n", 
                            i, i, (char*)list->cells[i].data,
                            list->cells[i].data,
                            list->cells[i].next, 
                            list->cells[i].prev);

        if(i != 1)
        {
            buff_pos += _shift(log_buffer + buff_pos, shift);

            buff_pos += sprintf(log_buffer + buff_pos, "cell_%d->cell_%d"
                                "[color=\"transparent\"];\n", i - 1, i);
        }

        if(list->cells[i].prev != -1 && list->cells[i].prev != 0)
        {
            buff_pos += _shift(log_buffer + buff_pos, shift);

            buff_pos += sprintf(log_buffer + buff_pos, "cell_%d:<prev>->cell_%d"
                                "[constraint=false];\n",
                                i, list->cells[i].prev);
        }

        if(list->cells[i].next != 0)
        {
            buff_pos += _shift(log_buffer + buff_pos, shift);
            
            buff_pos += sprintf(log_buffer + buff_pos, "cell_%d:<next>->cell_%d"
                                "[constraint=false];\n",
                                i, list->cells[i].next);
        }
    }

    buff_pos += sprintf(log_buffer + buff_pos, "}");

    FILE* output = fopen(pathname, "w");

    fprintf(output, "%s", log_buffer);

    fclose(output);
    
    char dot_cmd[256] = {};
    char dot_clear[256] = {};

    sprintf(dot_cmd, "dot -Tpdf %s > %s.pdf", pathname, pathname);

    system(dot_cmd);

    sprintf(dot_clear, "rm %s", pathname);

    system(dot_clear);

    memset(log_buffer, '\0', sizeof(log_buffer));

    buff_pos = 0;
}

void* ListGetElement(List* list, int location)
{
    return list->cells[location].data; 
}
