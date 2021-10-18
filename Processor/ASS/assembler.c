#include "assembler.h"
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
        int param = 0;
        char cmd[20] = {};

        int ret = sscanf(src->text[i].start, "%s %d", cmd, &param);
        
        if(ret == 2)
            if(!strcmp("push", cmd))
                sprintf(compiled_line, )
                
    }
}