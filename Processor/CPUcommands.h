// typedef struct {	const char* asm_code;
// 					int 		cpu_code;
// 					size_t 		num_of_args;
// 				} CPUCMD;

// CPUCMD CPU_commands[] = {
// 	{"hlt"	, 0, 0},
// 	{"in"	, 1, 0},
// 	{"out"	, 2, 0},
// 	{"push"	, 3, 2},
// 	{"pop"	, 4, 1},
// 	{"add"	, 5, 0},
// 	{"sub"	, 6, 0}
// };

// CPU_COMMAND(name, opcode, argc, code)

CPU_COMMAND(hlt, 0b1000, 0,
	{
		printf("Stopping CPU\n");
		DTOR(int, stk);
		return;
	}
)

CPU_COMMAND(push, 0b1, 1,
	{
		cpu->ip++;

		int arg = 0;
		if(type & REG) 		arg += cpu->regs[cpu->bytecode[cpu->ip++]];
		if(type & CONST) 	arg += cpu->bytecode[cpu->ip++];
		if(type & RAM)		arg =  cpu->ram[arg];

		printf("PUSH arg is %d\n", arg);
		PUSH(int, stk, arg); 
	}
)

CPU_COMMAND(pop, 0b10, 0,
	{
		int arg = POP(int, stk);

		cpu->ip++;

		if(type & RAM)
		{
			int addr = 0;
			if(type & REG) 		addr += cpu->regs[cpu->bytecode[cpu->ip++]];
			if(type & CONST)	addr += cpu->bytecode[cpu->ip++]; 
			cpu->ram[addr] = arg;
		}
		else if(type & REG)
		{
			int reg = cpu->bytecode[cpu->ip++];
			cpu->regs[reg] = arg;
		}
		else
//!!!!!!!!!!!!!!!!!!!!!!!ASK///////////////////////////////////////////////////
			pr_err(LOG_CONSOLE, "Runtime error: wrong POP argument");

	}
)

CPU_COMMAND(add, 0b11, 0,
	{
		int a = POP(int, stk);
		int b = POP(int, stk);
		PUSH(int, stk, a + b);

		cpu->ip++;
	}
)

CPU_COMMAND(sub, 0b100, 0,
	{
		int b = POP(int, stk);
		int a = POP(int, stk);

		PUSH(int, stk, a - b);

		cpu->ip++;
	}
)

CPU_COMMAND(mul, 0b101, 0,
	{
		int a = POP(int, stk);
		int b = POP(int, stk);
		PUSH(int, stk, a * b);

		cpu->ip++;
	}
)

CPU_COMMAND(in, 0b110, 0,
	{
		int input = 0;

		printf("input:");
		fflush(stdout);
		
		int res = scanf("%d", &input);

		if(!res)
		{
			pr_err(LOG_CONSOLE, "Runtime error: can't read input\n");
			exit(EXIT_FAILURE);
		}

		printf("\n");

		PUSH(int, stk, input);

		cpu->ip++;
	}
)

CPU_COMMAND(out, 0b111, 0,
	{
		int output = 0;
		output = POP(int, stk);	

		printf("output: %d\n", output);

		cpu->ip++;
	}
)

// CPU_REG(name, number)
CPU_REG(ax, 0b1)
