#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../lib/logger/logger.h"
#include "../lib/TextLib/Text.h"
#include "../lib/TextLib/File.h"

// #define StkElem int
// #include "../lib/Stack/Stack.h"


#include "cpu.h"

void parseCommands(const Text* txt);

int main(int argc, const char** argv)
{
    if(argc != 2)
    {
        pr_err(LOG_CONSOLE, "Enter file\n");
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];
    // Text* txt = text_init(filename);    

    // parseCommands(txt);
    CPU cpu  = {};
    $
    cpu.bytecode = readText(argv[1], fileSize(argv[1]));

    $
    exec(&cpu, fileSize(argv[1]));
    $
    // text_free(txt);
    log_close();
    
    return 0;
}

// void parseCommands(const Text* txt)
// {
    // STACK(int) stk = {};
    // CTOR(int, stk, 20);
// 
    // char cmd[20] = {};
    // int param = 0;
// 
// 
    // for(int i = 0; i < txt->num_of_lines; i++)
    // {
        // int ret = sscanf(txt->text[i].start, "%s %d", cmd, &param);
        // if(ret == 2)
            // if(!strcmp("push", cmd))
                // PUSH(int, stk, param);
        // 
        // if(ret == 1)
        // {
            // if(!strcmp("pop", cmd))
                // POP(int, stk);
            // 
            // if(!strcmp("sub", cmd))
            // {
                // int a = POP(int, stk);
                // int b = POP(int, stk);
                // PUSH(int, stk, b-a);
                // printf("sub: %d\n", b-a);
            // }
// 
            // if(!strcmp("mul", cmd))
            // {
                // int a = POP(int, stk);
                // int b = POP(int, stk);
                // PUSH(int, stk, a*b);
                // printf("mul: %d\n", a*b);
// 
            // }
// 
            // if(!strcmp("add", cmd))
            // {
                // int a = POP(int, stk);
                // int b = POP(int, stk);
                // PUSH(int, stk, a+b);
                // printf("add: %d\n", a+b);
// 
            // }
            // if(!strcmp("out", cmd))
            // {
                // int a = POP(int, stk);
                // printf("out : %d\n", a);
            // }
            // 
        // }
// 
    // }
    // DTOR(int, stk);
// }