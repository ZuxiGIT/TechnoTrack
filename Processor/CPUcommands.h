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
CPU_COMMAND(push, 0b1, 1,
	{
		;
	}
)

CPU_COMMAND(pop, 0b10, 0,
	{
		;
	}
)

CPU_COMMAND(add, 0b11, 0,
	{
		;
	}
)

CPU_COMMAND(sub, 0b100, 0,
	{
		;
	}
)

CPU_COMMAND(mul, 0b101, 0,
	{
		;
	}
)

CPU_COMMAND(in, 0b110, 0,
	{
		;
	}
)

CPU_COMMAND(out, 0b111, 0,
	{
		;
	}
)

CPU_COMMAND(hlt, 0b1000, 0,
	{
		;
	}
)
// CPU_REG(name, number)
CPU_REG(ax, 0b10)
