#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "../lib/TextLib/Text.h"

enum ASMERR
{
    OK,
    UNKNOWN_COMMAND
};

extern enum ASMERR asmerr;

enum Commands
{
    #define CPU_COMMAND(name, opcode, argc, code) CMD_##name = opcode,
	#define CPU_REG(name, number) 

    #include "../CPUcommands.h"

    #undef CPU_COMMAND
	#undef CPU_REG
};

typedef struct
{
    int addr;
    char* name;
} Label;

typedef struct 
{
    int addr;
    int label_num;
} FUrecord;

//const char* const sCommands[] = 
//{
//    #define CPU_COMMAND(name, opcode, argc, code) #name,
//	#define CPU_REG(name, number) 
//
//    #include "../CPUcommands.h"
//
//    #undef CPU_COMMAND
//	#undef CPU_REG
//};

enum Registers
{
	#define CPU_COMMAND(name, opcode, argc, code) 
	#define CPU_REG(name, number) name = number,

    #include "../CPUcommands.h"

	#undef CPU_COMMAND
    #undef CPU_REG
};

Text* compilation(Text* src);



#endif /* ASSEMBLER_H */
