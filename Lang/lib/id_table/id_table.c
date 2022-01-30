#include "id_table.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


void add_id_record(id_table_t* table, id_record_t record)
{
    assert(table != NULL);

    table->size++;

    if(table->records == NULL)
    {
        table->records = (id_record_t*)calloc(1, sizeof(id_record_t));
        memcpy(table->records, &record, sizeof(id_record_t));
        return;
    }

    id_record_t* last_record = table->records;

    while(last_record->next != NULL)
        last_record = last_record->next;

    last_record->next = (id_record_t*)calloc(1, sizeof(id_record_t));
    memcpy(last_record->next, &record, sizeof(id_record_t));
}


void clear_id_table(id_table_t* table)
{
    assert(table != NULL);

    id_record_t* current_record = table->records;
    while(current_record->next != NULL)
    {
        id_record_t* next = current_record->next;
        free(current_record);
        current_record = next;
    }
    free(current_record);
}

bool is_in_id_table(id_table_t* table, char* name)
{
    assert(table != NULL);
    assert(name != NULL);

    if(table->records == NULL)
        return false;

    id_record_t* current_record = table->records;
    while(current_record->next != NULL)
    {
        if(strncmp(current_record->name, name, strlen(current_record->name)) == 0)
                return true;
        current_record = current_record->next;
    }

    if(strncmp(current_record->name, name, strlen(current_record->name)) == 0)
            return true;

    return false;
}

id_record_t* get_last_record(id_table_t* table)
{
    assert(table != NULL);
    
    if(table->records == NULL)
        return NULL;

    id_record_t* current_record = table->records;

    while(current_record->next != NULL)
        current_record = current_record->next;

    return current_record;

}


void id_table_free(id_table_t* table)
{
    id_record_t* current_record = table->records;

    while(current_record->next != NULL)
    {
        id_record_t* next = current_record->next;
        free(current_record->name);
        free(current_record);
        current_record = next;
    }

    free(current_record->name);
    free(current_record);

}
