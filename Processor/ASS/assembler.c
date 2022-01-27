#include "assembler.h"
#include "../lib/logger/logger.h"

#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define $ printf("[%s:%d] processing line is \"%s\"\n", strrchr(__FILE__, '/') + 1, __LINE__, line); 
#define $$ printf("[%s:%d] CHECK\n", strrchr(__FILE__, '/') + 1, __LINE__);
#define $$$ printf("[%d] line is \"%s\"\n", __LINE__, line);

#define write_result_line(num, src)\
do{\
    output->text[i].start = strndup(src, num);\
    output->text[i].length = num;\
    output->text[i].finish = output->text[i].start + num;\
}while(0);

#define CONST           0x80
#define RAM             0x40
#define REG             0x20
#define JMP_COMMAND     0x10

enum ASMERR asmerr;

void fillMissedAddresses(FUrecord* table, Label* labels, Text* txt)
{
    for(int i = 0; i < 10; i ++)
        if(table[i].addr)
        {
            int addr = table[i].addr;
            for(int j = 0; j < txt->num_of_lines; j++)
                if(addr > txt->text[j].length)
                {
                    addr -= txt->text[j].length;
                }
                else
                {
                    printf("adding address %d for %s on %d\n", 
                            labels[table[i].label_num].addr,
                            labels[table[i].label_num].name,
                            table[i].addr);
                    txt->text[j].start[addr] = labels[table[i].label_num].addr;
                    break;
                }
        }
}

void addFixUpsTableRecord(FUrecord* table, int addr, int label_num)
{
    static int current_record = 0;
    table[current_record++] = (FUrecord) { addr, label_num };
}

int labelExists(Label* table, char* label)
{
    printf("searching label \'%s(%ld)\'\n", label, strlen(label));

    for(int i = 0; i < 10; i ++)
        if(table[i].addr)    
            if(!strncmp(table[i].name, label, strlen(label)))
                return i;

    return -1;

}

void freeLabelTable(Label* table)
{
    for(int i = 0; i < 10; i ++)
        if(table[i].name)
            free(table[i].name);
}

int hasArg(const char* cmd)
{
    #define CPU_COMMAND(name, opcode, argc, code) \
	if (!strcmp(#name, cmd) && argc >= 1 ) return argc;
	
	#define CPU_REG(name, number)
	
    #include "../CPUcommands.h"

	#undef CPU_COMMAND
	#undef CPU_REG
	
	return 0;
}

#define skipSpaces(txt) _skipSpaces(&txt)
#define skipAlphas(txt) _skipAlphas(&txt)
#define skipDigits(txt) _skipDigits(&txt)

static void _skipDigits(char** str)
{
	while(isdigit(*(*str))) (*str)++ ;
}

static void _skipSpaces(char** str)
{
	while(isspace(*(*str))) (*str)++ ;
}

static void _skipAlphas(char** str)
{
	while(isalpha(*(*str))) (*str)++ ;
}

int parseArg(char** _str, char* output_line)
{
    char* line = *_str;

    bool bracket = false;

    if(*line == '[')
    {
        bracket = true;
        output_line[0] |= RAM;
        line++;
    }

    skipSpaces(line);
    printf("line before parsing register \"%s\"\n", line);

    if(isalpha(*line))
    {
        printf("arg contains a register\n");

        #define CPU_COMMAND(name, opcode, argc, code)    
        #define CPU_REG(name, number) \
        if (!strncmp(#name, line, sizeof(#name) - 1))\
        { \
            output_line[0] |= REG;\
            output_line[1] = number;\
            skipAlphas(line);\
            skipSpaces(line);\
            if(bracket && *line == ']')\
            {\
                line++;\
                skipSpaces(line);\
		        if(*line != '\0' && *line != ';')\
                {\
                    pr_err(LOG_CONSOLE, "Syntax error\n");\
                    return -1;\
                }\
                else\
                    return 0;\
            }\
            if(bracket && *line++ != '+')\
            {\
                pr_err(LOG_CONSOLE, "Error ocurred: expected \'+\' "\
                                    "but got \'%c\'\n", *(line-1));\
                return -1;\
            }\
            else\
                skipSpaces(line);\
        }\
        else


        #include "../CPUcommands.h"

        {
            pr_err(LOG_CONSOLE, "Syntax error: Unknown register operand\n");
            return -1;
        }

        #undef CPU_COMMAND
        #undef CPU_REG
    }

    printf("line before parsing constant \"%s\"\n", line);

    if(isdigit(*line))
    {
        char arg = 0;	
        sscanf(line, "%hhd", &arg);
        if(output_line[0] & REG)
            output_line[2] = arg;	
        else
            output_line[1] = arg;

        printf("arg is %hhd\n", arg);

        output_line[0] |= CONST;

        skipDigits(line);
    }

    skipSpaces(line);

    $$$

    if(bracket)
        if(*line != ']') 
        {
            pr_err(LOG_CONSOLE, "Syntax error: expected \']\' but got \'%c\'\n", *line);
            return -1;
        }
        else
            line++;

    skipSpaces(line);

    $$$

    *_str = line;

    // means that there was no arg (because atributes REG, RAM, CONST are 0)
    if((output_line[0] & 0xe0) == 0)
    {
        pr_err(LOG_CONSOLE, "Error: expected an operand\n");
        return -1;
    }

    return 0;
}


#define printf(...)  printf(__VA_ARGS__)

Text* compilation(Text* src)
{
    assert(src);

    Text* output = (Text*)calloc(src->num_of_lines, sizeof(Text));

	output->text = (Line*)calloc(src->num_of_lines, sizeof(Line));
	output->num_of_lines = src->num_of_lines;

    Label labels[10] = {};
    FUrecord tableFU [10] = {};

    int current_address = 0;
    int current_label = 0;

    asmerr = OK;
    
    for(int i = 0; i < src->num_of_lines /*&& asmerr == OK*/; i++)
    {

        output->text[i].start = NULL;
        output->text[i].length = 0;
        output->text[i].finish = NULL; 
        
        char output_line[3] = {};
        char cmd[10] = {};

		char* line = src->text[i].start;

        skipSpaces(line);

        printf("[%d] line is \"%s\"\n", __LINE__, line);

        // for situations when label is used
        // label: 
        //  add
        //  sub
        //  jmp another_label

        // checking that is not a cmd, but a label
        // proccessing label (example "some_label:")
        if(strchr(line, ':'))
        {
            char format[10] = {};
            int offset = strchr(line, ':') - line;

            printf("------------->offset is %d\n", offset);

            // filling 'format' to scanf the  label
            sprintf(format, "%%%ds", offset);

            sscanf(line, format, cmd);

            printf("label is %s\n", cmd);

            int label_num = labelExists(labels, cmd);

            printf("label_num is %d\n", label_num);

            if(label_num != -1)
            {
                // label exist in label table
                printf("label address was changed from %d to %d in \'labels\'\n", labels[label_num].addr, current_address); 
                labels[label_num].addr = current_address;
            }
            else
            {
                // adding label to label table
                labels[current_label++] = (Label){current_address, strndup(cmd, offset)};
                printf("label \'%s\' with %d address was added\n", cmd, current_address);
            }

            continue;
        }
        //end processing label
        else
            sscanf(line, "%10s", cmd); //therefore it is a command
        
        printf("[%d] cmd is \"%s\"\n", __LINE__, cmd);

        #define CPU_COMMAND(name, opcode, argc, code) \
        if (!strncmp(#name, cmd, sizeof(#name) - 1))\
		{\
			output_line[0] |= opcode;\
            printf("opcode is 0x%hhx\n", opcode);\
			printf("cmd = %3s and name = %3s\n", cmd, #name);\
		}\
        else\

        #define CPU_REG(name, number)

        #include "../CPUcommands.h"

        if(*line == '\0' || *line == ';')
        {

            printf("[%d] Skipping the line\n", __LINE__);
            continue;
        }
        else
        {
                $$
				pr_err(LOG_CONSOLE, "Syntax error: Unknown command\n"
                                    "\t[Line:%d]-->\"%s\"\n", i, cmd);
				asmerr = UNKNOWN_COMMAND;
				continue;
        }

        #undef CPU_COMMAND
		#undef CPU_REG
        
        skipAlphas(line);

		skipSpaces(line);

        // if cmd is JMP
        //processing JMP's command arg
        if(output_line[0] & 0x1f == 0b10000 || 
           output_line[0] & 0x1f == 0b10001 || // jmp opcodes
           output_line[0] & 0x1f == 0b10010 ||
           output_line[0] & 0x1f == 0b10011 ||
           output_line[0] & 0x1f == 0b10100 ||
           output_line[0] & 0x1f == 0b10101 ||
           output_line[0] & 0x1f == 0b10110)
        {
            printf("parsing cmd: %s\n", cmd);

            int found_label = -1;

            found_label = labelExists(labels, line);

            if(found_label != -1)
            // label was found
            {
                printf("label was found\n");

                output_line[1] = labels[found_label].addr;        

                output->text[i].start = strndup(output_line, 2);
                output->text[i].length = 2;
                output->text[i].finish = output->text[i].start + 2; 

                current_address += 2;
            }
            //label was not found
            else
            {
                current_address++;

                addFixUpsTableRecord(tableFU, current_address, current_label);

                printf("added record with \'addres\':%d and \'label_num\':%d params\n", current_address, current_label); 
                
                current_address++;

                labels[current_label++] = (Label){ -1, strndup(line, strlen(line))};

                printf("added %s label with address -1 in \'labels\'\n", line);

                output_line[1] = -1;        

                output->text[i].start = strndup(output_line, 2);
                output->text[i].length = 2;
                output->text[i].finish = output->text[i].start + 2; 
            }

            printf("entered line: %s\n\toutputline: 0x%hhX 0x%hhX 0x%hhX\n", src->text[i].start, output_line[0], output_line[1], output_line[2]);
            
            continue; 
        } // end processing JMP's command arg

        $$

        $

        //proccessing cmd's arg
        int num_of_args = hasArg(cmd);
		if(num_of_args)
		{

            if(parseArg(&line, output_line) < 0)
            {
                printf("\t[Line:%d] ---> \"%s\"\n", i+1, src->text[i].start);
                continue;
            }
            else
            {
                pr_warn(LOG_CONSOLE, "output_line[0] = 0x%hhx\n", output_line[0]);
                // checking the first "move" operand if it is a const
                if((output_line[0] & 0xf) == 0b1000) // --> "move" opcode
                    if((output_line[0] & CONST) && !(output_line[0] & RAM))
                    {
                        pr_err(LOG_CONSOLE, "Error: \"move\" cannot have the first"
                                            " as an immediate\n");
                        printf("\t[Line:%d] ---> \"%s\"\n", i+1, src->text[i].start);
                        continue;
                    }
            }

            if(num_of_args > 1)
            {
                skipSpaces(line);

                if(*line != ',')
                {
                    pr_err(LOG_CONSOLE, "Syntax error: wrong delimeter "
                                        "between operands\n");
                    printf("\t[Line:%d] ---> \"%s\"\n", i+1, src->text[i].start);
                    continue;
                }

                line++;
                skipSpaces(line);

                char output_line_4_2_arg[3] = {};

                if(parseArg(&line, output_line_4_2_arg) < 0)
                {
                    printf("\t[Line:%d] ---> \"%s\"\n", i+1, src->text[i].start);
                    continue;
                }
                else
                {
                    if(num_of_args == 2)
                        if(*line != '\0' && *line != ';')
                        {
                            pr_err(LOG_CONSOLE, "Syntax error\n");
                            printf("\t[Line:%d] ---> \"%s\"\n", i+1, src->text[i].start);
                           continue; 
                        }

                    pr_warn(LOG_CONSOLE, "output_line[0] = 0x%hhx\n", output_line[0]);
                    char result_line[5] = {};
                    int num_of_bytes = 1;

                    if(output_line[0] & REG)
                        num_of_bytes++;
                    if(output_line[0] & CONST)
                        num_of_bytes++;

                    memcpy(result_line, output_line, num_of_bytes);
                    int temp = num_of_bytes;

                    if(output_line_4_2_arg[0] & REG)
                        num_of_bytes++;
                    if(output_line_4_2_arg[0] & CONST)
                        num_of_bytes++;


                    memcpy(result_line + temp, output_line_4_2_arg + 1, num_of_bytes - temp);
                    
                    pr_warn(LOG_CONSOLE, "result_line[0] = 0x%hhx\n", result_line[0]);
                    // for move operands only 
                    if((output_line[0] & 0xf) == 0b1000) // --> "move" opcode
                    {
                        result_line[0] |= (char)(((long long)output_line_4_2_arg[0] & REG) >> 5);
                        result_line[0] |= (char)(((long long)output_line_4_2_arg[0] & RAM) >> 5);
                        result_line[0] |= (char)(((long long)output_line_4_2_arg[0] & CONST) >> 5);
                    }

                    pr_info(LOG_CONSOLE, "num is %d\n", num_of_bytes);
                    pr_warn(LOG_CONSOLE, "result_line[0] = 0x%hhx\n", result_line[0]);

                    write_result_line(num_of_bytes, result_line);

                    printf("entered line: %s\n\toutputline: 0x%hhX 0x%hhX"
                            " 0x%hhX 0x%hhX 0x%hhX\n",
                            src->text[i].start,
                            result_line[0], result_line[1], result_line[2],
                            result_line[3], result_line[4]);

                    continue;
                }

                skipSpaces(line);
            }
		}


		if(*line != '\0' && *line != ';' && asmerr == OK)
			pr_err(LOG_CONSOLE, "Syntax error on line %d: %s\n",
                                i, src->text[i].start);

        printf("entered line: %s\n\toutputline: 0x%hhX 0x%hhX 0x%hhX\n",
                src->text[i].start,
                output_line[0], output_line[1], output_line[2]);
	
		if((output_line[0] & REG) && (output_line[0] & CONST))
		{
            write_result_line(3, output_line);
			//output->text[i].start = strndup(output_line, 3);
			//output->text[i].length = 3;
			//output->text[i].finish = output->text[i].start + 3;
		}
		else if((output_line[0] & REG) | (output_line[0] & CONST))
		{		
            write_result_line(2, output_line);
			//output->text[i].start = strndup(output_line, 2);
			//output->text[i].length = 2;
			//output->text[i].finish = output->text[i].start + 2;
		}		
		else if (output_line[0] & 0x1f)
		{
            write_result_line(1, output_line);
			//output->text[i].start = strndup(output_line, 1);
			//output->text[i].length = 1;
			//output->text[i].finish = output->text[i].start + 1;
		}

        current_address += output->text[i].length;
    }
	
    fillMissedAddresses(tableFU, labels, output);
    freeLabelTable(labels);

	if(asmerr != OK)
	{
		text_free(output);
		return NULL;
	}

	return output;
}

#undef skipDigits
#undef skipAlphas
#undef skipSpaces
#undef $$$
#undef $$
#undef $
