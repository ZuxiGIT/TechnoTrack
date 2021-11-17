#ifndef TEXT_H
#define TEXT_H
#include "Strings.h"
#include <stddef.h>

/*! \file */

typedef struct
{
    Line* text;
    int num_of_lines;
} Text;

void text_free(Text* text);
Text* text_init(const char* path);
#endif /* TEXT_H */