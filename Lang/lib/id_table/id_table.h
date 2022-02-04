#ifndef ID_TABLE_H
#define ID_TABLE_H

#include <stdbool.h>

typedef struct id_record_t
{
    struct id_record_t* next;
    char* name;
    int offset;
} id_record_t;

typedef struct id_table_t
{
    id_record_t* records;
    int size;
} id_table_t;

void add_id_record(id_table_t* table, id_record_t record);
id_record_t* get_last_record(id_table_t* table);

bool is_in_id_table(id_table_t* table, char* name);

void id_table_free(id_table_t* table);
void clear_id_table(id_table_t* table);

void print_id_table(id_table_t* table);

#endif /* ID_TABLE_H */
