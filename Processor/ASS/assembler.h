#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "../lib/TextLib/Text.h"

enum Commands
{
    #define CPU_COMMAND(name, opcode, argc, code) CMD_##name = opcode,

    #include "../CPUcommands.h"

    #undef CPU_COMMAND
};

const char* const sCommands[] = 
{
    #define CPU_COMMAND(name, opcode, argc, code) #name,

    #include "../CPUcommands.h"

    #undef CPU_COMMAND
};

Text* compilation(Text* src);



#endif /* ASSEMBLER_H */