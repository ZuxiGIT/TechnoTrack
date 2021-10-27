#include "assembler.h"
#include "../lib/logger/logger.h"

#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define RAM             0x80
#define REG             0x40
#define CONST           0x20
#define JMP_COMMAND     0x10

enum ASMERR asmerr;

void freeLabelTable(Label* table)
{
    for(int i = 0; i < 10; i ++)
        if(table[i].name)
            free(table[i].name);
}

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

void skipAlphas(char** str)
{
	while(isalpha(*(*str))) (*str)++ ;
}

#define printf printf

Text* compilation(Text* src)
{
    assert(src);

    Text* output = (Text*)calloc(src->num_of_lines, sizeof(Text));

	output->text = (Line*)calloc(src->num_of_lines, sizeof(Line));
	output->num_of_lines = src->num_of_lines;

    Label labels[10] = {};

    int current_address = 0;
    int current_label = 0;

    asmerr = OK;
    
    for(int i = 0; i < src->num_of_lines; i++)
    {

        output->text[i].start = NULL;
        output->text[i].length = 0;
        output->text[i].finish = NULL; 
        
        char output_line[3] = {};
        char cmd[10] = {};

		char* line = src->text[i].start;

        printf("[%d] line is \"%s\"\n", __LINE__, line);

        if(strchr(line, ':'))
        {
            char format[10] = {};
            int offset = strchr(line, ':') - line;
            printf("------------->offset id %d\n", offset);
            sprintf(format, "%%%ds", offset);
            sscanf(line, format, cmd);
            printf("label is %s\n", cmd);
            labels[current_label++] = (Label){current_address, strndup(cmd, offset)};
            printf("label \'%s\' with %d address was added\n", cmd, current_address);
            continue;
        }
        else
            sscanf(line, "%10s", cmd);
        
        printf("[%d] cmd is \"%s\"\n", __LINE__, cmd);

        #define CPU_COMMAND(name, opcode, argc, code) \
        if (!strcmp(#name, cmd))\
		{\
			output_line[0] |= opcode;\
			printf("cmd = %3s and name = %3s\n", cmd, #name);\
		}
        #define CPU_REG(name, number)

        #include "../CPUcommands.h"

        #undef CPU_COMMAND
		#undef CPU_REG

        if(output_line[0] & JMP_COMMAND)
        {
            printf("parsing cmd:%s arg\n", cmd);

            while(isalpha(*line)) line++;
            
            skipSpaces(&line);

            for(int j =  0; j < current_label; j ++)
            {
                printf("label is %s\n", labels[j].name);
                if(!strncmp(labels[j].name, line, strlen(labels[j].name)))
                {
                    printf("label was found\n");

                    output_line[1] = labels[j].addr;        

                    output->text[i].start = strndup(output_line, 2);
                    output->text[i].length = 2;
                    output->text[i].finish = output->text[i].start + 2; 

                    current_address += 2;
                    
                    printf("entered line: %s\n\toutputline: 0x%hhX 0x%hhX 0x%hhX\n", src->text[i].start, output_line[0], output_line[1], output_line[2]);

                    break;
                }
            }
            continue; 
        }

		printf("[Line:%d] File: %s: CHECK\n", __LINE__, __FILE__);
		fflush(stdout);

		if(isalpha(*line))	
			if(output_line[0] == 0)
			{
				printf("[Line:%d] File: %s: CHECK\n", __LINE__, __FILE__);
				fflush(stdout);
				pr_err(LOG_CONSOLE, "Syntax error: Unknown command\n[Line:%d]-->%s\n", i, src->text[i].start);
				printf("[Line:%d] File: %s: CHECK\n", __LINE__, __FILE__);
				fflush(stdout);
				asmerr = UNKNOWN_COMMAND;
				printf("[Line:%d] File: %s: CHECK\n", __LINE__, __FILE__);
				fflush(stdout);
				// continue;
			}

		while(isalpha(*line)) line++;

		skipSpaces(&line);


        printf("[%d] line is \"%s\"\n", __LINE__, line);



        printf("[%d] line is \"%s\"\n", __LINE__, line); // define

		if(hasArg(cmd))
		{

			if(*line == '[')
			{
				output_line[0] |= RAM;
				line++;
			}

            printf("line before parsing reg \"%s\"\n", line);

			#define CPU_COMMAND(name, opcode, argc, code)
			#define CPU_REG(name, number) \
			if (!strncmp(#name, line, sizeof(#name) - 1))\
			{ \
				output_line[0] |= REG;\
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
				if(output_line[0] & REG)
					output_line[2] = arg;	
				else
					output_line[1] = arg;

                printf("arg is %hhd\n", arg);

				output_line[0] |= CONST;

				while(isdigit(*line)) line++;
			}

			skipSpaces(&line);

        	printf("[%d] line is \"%s\"\n", __LINE__, line);

			if(output_line[0] & RAM)
				if(*line != ']') 
				{
					pr_err(LOG_CONSOLE, "Syntax error: expected \']\' but got %c\n", *line);
					continue;
				}
				else
					line++;

			skipSpaces(&line);

        	printf("[%d] line is \"%s\"\n", __LINE__, line);
		}


		if(*line != '\0' && *line != ';' && asmerr == OK)
			pr_err(LOG_CONSOLE, "Syntax error\n[Line:%d]-->%s\n", i, src->text[i].start);

        printf("entered line: %s\n\toutputline: 0x%hhX 0x%hhX 0x%hhX\n", src->text[i].start, output_line[0], output_line[1], output_line[2]);
	
		if(output_line[0] & REG && output_line[0] & CONST)
		{
			output->text[i].start = strndup(output_line, 3);
			output->text[i].length = 3;
			output->text[i].finish = output->text[i].start + 3;
		}
		else if((output_line[0] & REG) | (output_line[0] & CONST))
		{		
			output->text[i].start = strndup(output_line, 2);
			output->text[i].length = 2;
			output->text[i].finish = output->text[i].start + 2;
		}		
		else if (output_line[0] & 0x1f)
		{
			output->text[i].start = strndup(output_line, 1);
			output->text[i].length = 1;
			output->text[i].finish = output->text[i].start + 1;

		}

        current_address += output->text[i].length;
    }
	
	log_close();
	
    freeLabelTable(labels);

	if(asmerr != OK)
	{
		text_free(output);
		return NULL;
	}
	return output;
}
