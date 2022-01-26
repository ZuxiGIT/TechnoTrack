#ifndef CPU_H
#define CPU_H


#define $ printf("%s[%d]\n", __FILE__, __LINE__);

enum REG
{
    #define CPU_COMMAND(cmd_name, opcode, argc, code)
    #define CPU_REG(reg_name, number)\
    reg_name = number,

    #include "../CPUcommands.h"

    #undef CPU_COMMAND
    #undef CPU_REG 
};

typedef struct
{
    char regs[4];
    char ram[1024];
    int ip;
    unsigned char* bytecode;
    int bytecode_len;

} CPU;


void exec(CPU* cpu, int size);
#endif
