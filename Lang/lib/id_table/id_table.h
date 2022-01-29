#ifndef ID_TABLE_H
#define ID_TABLE_H

#include <stdbool.h>

typedef struct id_record_t
{
    char* name;
    int offset;
    struct id_record_t* next;
} id_record_t;

typedef struct
{
    id_record_t* records;
    int size;
} id_table_t;

void add_id_record(id_table_t* table, id_record_t record);
void clear_id_table(id_table_t* table);
bool is_in_id_table(id_table_t* table, char* name);

#endif /* ID_TABLE_H */
