#include "list.h"
#include "./logger/logger.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define VERIFY_CELL(loc)\
    if(list->cells[loc].prev == -1)\
    {\
        pr_err(LOG_CONSOLE, "Bad location "\
               "(cell with %d address does not exist\n",\
                loc);\
        return -1;\
    }\
\
    if(!loc)\
    {\
        pr_err(LOG_CONSOLE, "Bad location "\
               "(location can not be equal 0)\n");\
        return -1;\
    }\

void ListCtor(list_t* list, int capacity)
{
    list->cells = (cell_t*)calloc(capacity, sizeof(cell_t));

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

    list->resizeable = false;
}

void ListDtor(list_t* list)
{
    free(list->cells);
}

int ListInsertBack(list_t* list, void* data)
{
    if(!list->head)
    {
        list->cells[1].data = data;
        list->head = list->tail = 1;
        list->cells[1].next = 0;
        list->cells[1].prev = 0;
        list->free = 2;
        list->size++;
        return 1; 
    }
    
    return ListInsertAfter(list, list->tail, data);
}

int ListInsertFront(list_t* list, void* data)
{
    if(!list->head)
    {
        list->cells[1].data = data;
        list->head = list->tail = 1;
        list->cells[1].next = 0;
        list->cells[1].prev = 0;
        list->free = 2;
        list->size++;
        return 1; 
    }

    return ListInsertBefore(list, list->head, data);
}

int ListInsertAfter(list_t* list, int location, void* data)
{
    VERIFY_CELL(location);

    int found = list->free;

    list->free = list->cells[found].next;

    //printf("found cell with %d address (location = %d, num = %d)\n",
    //        found, location, num);

    if(found)
    {
        list->cells[found].data = data;

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
    {
        if(list->resizeable)
        {
            ListResize(list, (int)(list->capacity * 1.67));
            found = ListInsertAfter(list, location, data);
        }
        else
            pr_err(LOG_CONSOLE, "List is full\n");
    }
    
    return found;
} 

int ListInsertBefore(list_t* list, int location, void* data)
{
    VERIFY_CELL(location);

    int found = list->free;

    list->free = list->cells[found].next;

    if(found)
    {
        list->cells[found].data = data;
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
    {
        if(list->resizeable)
        {
            ListResize(list, (int)(list->capacity * 1.67));
            found = ListInsertBefore(list, location, data);
        }
        else
            pr_err(LOG_CONSOLE, "List is full\n");
    }

    return found;
}

int ListDelete(list_t* list, int location)
{
    VERIFY_CELL(location);

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
    
    return list->cells[location].data;
}

int ListPopFront(list_t* list)
{
    return ListDelete(list, list->head);
}

int ListPopBack(list_t* list)
{
    return ListDelete(list, list->tail);
}

int ListDeleteAfter(list_t* list, int location)
{
    VERIFY_CELL(location);
    
    if(location == list->tail)
    {
        pr_err(LOG_CONSOLE, "Bad location "
               "(cell after cell with address %d does not exist\n", location);
        return (unsigned)-1;
    }

    return ListDelete(list, list->cells[location].next);
}

int ListDeleteBefore(list_t* list, int location)
{
    VERIFY_CELL(location);

    if(location == list->head)
    {
        pr_err(LOG_CONSOLE, "Bad location "
               "(cell before cell with address %d does not exist\n", location);
        return (unsigned)-1;
        
    }

    return ListDelete(list, list->cells[location].prev);
}

void PrintList(list_t* list)
{
    //printf("list.head = %d\tlist.tail = %d\tlist.free = %d\n",
    //        list->head, list->tail, list->free);

    for(int i = 0; i < list->capacity; i ++)
    {
        //printf("Cell[%d]: data = %d\tnext = %d\tprev = %d\n",
        //        i, list->cells[i].data, list->cells[i].next, 
        //        list->cells[i].prev);
    }
}

static int _shift(char* buff, int shift)
{
    for(int i = 0; i < shift; i++)
        sprintf(buff + i, "\t");

    return shift;
}

void LogList(const char* pathname, list_t* list)
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
                            "label=\"{ addres\\n%d | data\\n%d }| <next> next\\n%d |"
                            "<prev> prev\\n%d \"];\n", 
                            i, i, list->cells[i].data, list->cells[i].next, 
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


void ListResize(list_t* list, int new_size)
{
    cell_t* tmp = (cell_t*)calloc(new_size, sizeof(cell_t));

    memcpy(tmp, list->cells, list->size);

    list->capacity = new_size;

    free(list->cells);
    list->cells = tmp;
}
