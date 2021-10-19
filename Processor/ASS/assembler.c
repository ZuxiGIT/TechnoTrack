#include "assembler.h"
#include <string.h>
#include <assert.h>
#include <malloc.h>

Text* compilation(Text* src)
{
    assert(!src);

    Text* output = (Text*)calloc(src->num_of_lines, sizeof(Line));

    char compiled_line[30] = {};
    int shift = 0;

    for(int i = 0; i < src->num_of_lines; i++)
    {
        
        char output[3] = {};
        char cmd[10] = {};

        sscanf(src->text[i].start, "%s", cmd);
        
        #define CPU_COMMAND(name, opcode, argc, code) \
        if (!strcmp(#name, cmd)) output[0] |= opcode;

        #include "../CPUcommands.h"

        #undef CPU_COMMAND
    }
}