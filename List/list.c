#include "list.h"
#include "./logger/logger.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

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

int ListInsertBack(List* list, int num)
{
    /*if(!list->head)
    {
        list->cells[1].data = num;
        list->head = list->tail = 1;
        list->cells[1].next = 0;
        list->cells[1]
        list->free = 2;
        return 1; 
    }
    */
    
    printf("searching new cell\n");

    int found = _findEmptyCell(list);

    if(found)
    {
        printf("Found empty cell %d\n", found);

        list->cells[found].data = num;
        if(list->tail)
            list->cells[list->tail].next = found;
        list->cells[found].next = 0;
        list->cells[found].prev = list->tail;
        list->tail = found;

        if(!list->head)
            list->head = found;
    }
    else
        pr_err(LOG_CONSOLE, "List is full\n");

    return found;
}

int ListInsertAfter(List* list, int location, int num)
{
    if(list->cells[location].prev == -1)
    {
        pr_err(LOG_CONSOLE, "Bad location (cell with %d does not exist\n",
                location);
        return -1;
    }

    if(!location)
    {
        pr_err(LOG_CONSOLE, "Bad location (location can be equal 0)\n");
        return -1;
    }

    int found = _findEmptyCell(list);


    if(found)
    {
        list->cells[found].next = list->cells[location].next;
        list->cells[list->cells[location].next].prev = found;
        list->cells[location].next = found;
        list->cells[found].data = num;
        list->cells[found].prev = location;
    }
    else
        pr_err(LOG_CONSOLE, "List is full\n");
    
    return found;    
} 

void PrintList(List* list)
{
    printf("list.head = %d\tlist.tail = %d\tlist.free = %d\n",
            list->head, list->tail, list->free);
    for(int i = 0; i < list->capacity; i ++)
    {
        printf("Cell[%d]: data = %d\tnext = %d\tprev = %d\n",
                i, list->cells[i].data, list->cells[i].next, 
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
}
