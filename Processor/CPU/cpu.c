#include "cpu.h"
#include <stdio.h>


#define CONST 	0x80
#define RAM 	0x40
#define REG 	0x20

#define StkElem int
#include "../lib/Stack/Stack.h"

void exec(CPU* cpu, int length)
{
    if(!cpu->bytecode)
        return;

    STACK(int) stk = {};
    CTOR(int, stk, 10);

    for(cpu->ip = 0; cpu->ip < cpu->bytecode_len; )
    {
        int type = cpu->bytecode[cpu->ip] & 0xE0;
        int code = cpu->bytecode[cpu->ip] & 0x1f;
        #define CPU_COMMAND(cmd_name, opcode, argc, _code)\
        if((cpu->bytecode[cpu->ip] & 0x1f) == opcode)\
        {\
            printf("current opcode 0x%x [ip  = %d]\n", cpu->bytecode[cpu->ip],\
                    cpu->ip);\
            _code;\
        }\
        else

        #define CPU_REG(reg_name, number)

        #include "../CPUcommands.h"

        {
            pr_err(LOG_CONSOLE, "Runtime error: Unknown command"
                                " [bytecode = 0x%x ip = %d]\n",
                                cpu->bytecode[cpu->ip],
                                cpu->ip); 
            abort();
        }

        #undef CPU_COMMAND
        #undef CPU_REG 
    }

    pr_err(LOG_CONSOLE, "Runtime error: no hlt [ip = %d bc_l = %d]\n", cpu->ip, cpu->bytecode_len);
    abort();

}
