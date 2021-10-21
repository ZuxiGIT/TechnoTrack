#include "assembler.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>





Text* compilation(Text* src)
{
    assert(!src);

    Text* output = (Text*)calloc(src->num_of_lines, sizeof(Line));

    char compiled_line[30] = {};
    int shift = 0;

    for(int i = 0; i < src->num_of_lines; i++)
    {
        
        char output_line[3] = {};
        char cmd[10] = {};
	char* line = src->text[i].start;

        sscanf(line, "%s", cmd);
        
        #define CPU_COMMAND(name, opcode, argc, code) \
        if (!strcmp(#name, cmd)) output[0] |= opcode;

        #include "../CPUcommands.h"

        #undef CPU_COMMAND

	while(isalpha(*line++)) ;
	while(isspace(*line++)) ;

	char arg = 0;	
	if(sscanf(line, "%hhd", &arg))
	{
		output_line[1] = arg;	
		output_line[0] |= 0x20;
	}
	

    }
}
