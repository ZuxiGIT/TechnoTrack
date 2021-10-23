#include "assembler.h"
#include "../lib/logger/logger.h"

#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


enum ASMERR asmerr;

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

    Text* output = (Text*)calloc(src->num_of_lines, sizeof(Text));

	output->text = (Line*)calloc(src->num_of_lines, sizeof(Line));
	output->num_of_lines = src->num_of_lines;

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

		if(output_line[0] == 0)
		{
			pr_err(LOG_CONSOLE, "Syntax error: Unknown command\n[Line:%d]-->%s\n", i, src->text[i].start);
			asmerr = UNKNOWN_COMMAND;
			continue;
		}

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
				while(isalpha(*line)) line++;\
				skipSpaces(&line);\
				if(*line++ != '+')\
				{\
					pr_err(LOG_CONSOLE, "Error ocurred: expected \'+\' but got %c\n"\
										"[Line:%d]--->%s\n", *line, i, src->text[i].start);\
				continue;\
				}\
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

				while(isdigit(*line)) line++;
			}

			skipSpaces(&line);

        	printf("[%d] line is \"%s\"\n", __LINE__, line);

			if((output_line[0] & 0x40) && *line != ']') 
			{
				pr_err(LOG_CONSOLE, "Syntax error: expected \']\' but got %c\n", *line);
				continue;
			}
			else
				line++;

			skipSpaces(&line);

        	printf("[%d] line is \"%s\"\n", __LINE__, line);
			if(*line != '\0' && *line != ';')
				pr_err(LOG_CONSOLE, "Syntax error\n[Line:%d]-->%s\n", i, src->text[i].start);

            printf("entered line: %s\n\toutputline: 0x%X 0x%X 0x%X\n", src->text[i].start, output_line[0], output_line[1], output_line[2]);

		}
		/*
			output->text[i].start = strndup(output_line, 1);
			output->text[i].length = 1;
			output->text[i].finish = output->text[i].start + 1;
			continue;

		*/

    }
	if(asmerr != OK)
		text_free(output);
}
