#include "cpu.h"
#include <stdio.h>


#define RAM 	0x80
#define REG 	0x40
#define CONST 	0x20

#define StkElem int
#include "../lib/Stack/Stack.h"

void exec(CPU* cpu, int length)
{
    if(!cpu->bytecode)
        return;

    STACK(int) stk = {};
    CTOR(int, stk, 10);

    for(cpu->ip = 0; cpu->ip < length;)
    {
        int type = cpu->bytecode[cpu->ip] & 0xE0;
        int code = cpu->bytecode[cpu->ip] & 0x1f;
        #define CPU_COMMAND(cmd_name, opcode, argc, code)\
        if((cpu->bytecode[cpu->ip] & 0x1f) == opcode)\
        code
        #define CPU_REG(reg_name, number)

        #include "../CPUcommands.h"

        #undef CPU_COMMAND
        #undef CPU_REG 
    }

    DTOR(int, stk);
}