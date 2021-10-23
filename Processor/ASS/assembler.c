#include "assembler.h"
#include "../lib/logger/logger.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>


bool hasArg(const char* cmd)
{
    #define CPU_COMMAND(name, opcode, argc, code) \
	if (!strcmp(#name, cmd) && argc >= 1 ) return 1;
	
	#define CPU_REG(name, number)
	
    #include "../CPUcommands.h"

	#undef CPU_COMMAND
	#undef CPU_REG
	return 0;
	
}

void skipSpaces(char** str)
{
	while(isspace(*(*str))) (*str)++ ;
}


Text* compilation(Text* src)
{
    assert(src);

    Text* output = (Text*)calloc(src->num_of_lines, sizeof(Line));


    for(int i = 0; i < src->num_of_lines; i++)
    {
        
        char output_line[3] = {};
        char cmd[10] = {};
		char* line = src->text[i].start;

        printf("[%d] line is \"%s\"\n", __LINE__, line);
        sscanf(line, "%s", cmd);
        
        #define CPU_COMMAND(name, opcode, argc, code) \
        if (!strcmp(#name, cmd)) output_line[0] |= opcode;

		#define CPU_REG(name, number)

        #include "../CPUcommands.h"

        #undef CPU_COMMAND
		#undef CPU_REG

        printf("[%d] line is \"%s\"\n", __LINE__, line);

		while(isalpha(*line)) line++;

		skipSpaces(&line);

        printf("[%d] line is \"%s\"\n", __LINE__, line);

		if(hasArg(cmd))
		{

			if(*line == '[')
			{
				output_line[0] |= 0x40;
				line++;
			}


            printf("line before parsing reg \"%s\"\n", line);

			#define CPU_COMMAND(name, opcode, argc, code)
			#define CPU_REG(name, number) \
			if (!strncmp(#name, line, sizeof(#name) - 1))\
			{ \
				output_line[0] |= 0x10;\
				output_line[1] = number;\
				skipSpaces(&line);\
				if(*line++ != '+')\
					pr_err(LOG_CONSOLE, "line %d\n\t%s\n", i, src->text[i].start);\
				skipSpaces(&line);\
			}

			#include "../CPUcommands.h"

			#undef CPU_COMMAND
			#undef CPU_REG
			
            printf("line before parsing constant \"%s\"\n", line);
			char arg = 0;	
			if(sscanf(line, "%hhd", &arg))
			{
				if(output_line[0] & 0x10)
					output_line[2] = arg;	
				else
					output_line[1] = arg;
                printf("arg is %hhd\n", arg);
				output_line[0] |= 0x20;
			}
	
            //printf("entered line: %s\n\toutputline: 0x%X 0x%X 0x%X\n", src->text[i].start, output_line[0], output_line[1], output_line[2]);

		}
		/*
			output->text[i].start = strndup(output_line, 1);
			output->text[i].length = 1;
			output->text[i].finish = output->text[i].start + 1;
			continue;

		*/
    }
}
