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




<<<<<<< HEAD
CPU_COMMAND( push, 0b1, 1,
=======
CPU_COMMAND(
	push,
	0b1,
	1,
>>>>>>> 7d853e484877dca50bc5ab003bd85c5a0f41fc3e
	{
		;
	}
)
<<<<<<< HEAD

CPU_REG(abx, 0b10)
=======
>>>>>>> 7d853e484877dca50bc5ab003bd85c5a0f41fc3e
