
// ---------------table of opcodes---------------
// ------reserved opcode------
// 0b00000 --> do not use it
// ------used opcodes------
// 0b00001 --> push
// 0b00010 --> pop
// 0b00011 --> add
// 0b00100 --> sub
// 0b00101 --> mul
// 0b00110 --> in
// 0b00111 --> out
// 0b01000 --> move empty command, reserved by move             --+-- has no code section 
// 0b01001 --> move (if the second operand is i.e. cx)          --+
// 0b01010 --> hlt                                                |
// 0b01011 --> move (if the second opernad is i.e. [cx])        --+--- for all of them the code section is almost the same
// 0b01100 --> move (if the second operand is i.e. 4 (imm))     --+
// 0b01101                                                        |
// 0b01110 --> move (if the second operand is i.e. [4])         --+
// 0b01111 --> move (if the second operand is i.e. [cx+3])      --+
// 0b10000 --> jmp
// 0b10001 --> je
// 0b10010 --> jne
// 0b10011 --> jg
// 0b10100 --> jge
// 0b10101 --> jl
// 0b10110 --> jle
// 0b10111
// 0b11000
// 0b11001
// 0b11010
// 0b11011
// 0b11100
// 0b11101
// 0b11110
// 0b11111
// ------free opcodes------
// 0b01101
// 0b10111
// 0b11000
// 0b11001
// 0b11010
// 0b11011
// 0b11100
// 0b11101
// 0b11110
// 0b11111
//
// CPU_COMMAND(name, opcode, argc, code)

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

CPU_COMMAND(pop, 0b10, 1,
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

CPU_COMMAND(hlt, 0b1010, 0,
	{
		pr_info(LOG_CONSOLE, "Stopping CPU\n");
		DTOR(int, stk);
		return;
	}
)

// move dest, src
// dest -> ram, reg
// src -> imm, reg, ram

CPU_COMMAND(move, 0b01000, 2,
    {
        pr_err(LOG_CONSOLE, "Runtime error:"
                            " wrong command\n");
        abort();
    }
)

// the second operand of move is const (imm)
CPU_COMMAND(move_imm, 0b01100, 2,
    {
        char first_operand_atr = cpu->bytecode[cpu->ip] & 0xe0;

        if(first_operand_atr & RAM)
        {
            if(first_operand_atr & REG)
            {
                if(first_operand_atr & CONST)
                {
                    int arg = cpu->bytecode[cpu->ip + 3];
                    int reg = cpu->bytecode[cpu->ip+1];
                    int imm = cpu->bytecode[cpu->ip+2];
                    cpu->ram[cpu->regs[reg] + imm] = arg;

                    cpu->ip += 4;
                }
                else
                {
                    int arg = cpu->bytecode[cpu->ip + 2];
                    int reg = cpu->bytecode[cpu->ip+1];
                    cpu->ram[cpu->regs[reg]] = arg;

                    cpu->ip += 3;
                }
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: cannot"
                                    " interpret command\n");
                abort();
            }
        }
        else
        {
            if(first_operand_atr & REG)
            {
                int arg = cpu->bytecode[cpu->ip + 2];
                int reg = cpu->bytecode[cpu->ip+1];
                cpu->regs[reg] = arg;
 
                cpu->ip += 3;
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: wrong"
                                    " command format\n");
                abort();
            }
        }
    }
)

//the second operand of move is register
CPU_COMMAND(move_reg, 0b01001, 2,
    {
        char first_operand_atr = cpu->bytecode[cpu->ip] & 0xe0;

        if(first_operand_atr & RAM)
        {
            if(first_operand_atr & REG)
            {
                if(first_operand_atr & CONST)
                {
                    int arg = cpu->regs[cpu->bytecode[cpu->ip + 3]];
                    int reg = cpu->bytecode[cpu->ip+1];
                    int imm = cpu->bytecode[cpu->ip+2];
                    cpu->ram[cpu->regs[reg] + imm] = arg;

                    cpu->ip += 4;
                }
                else
                {
                    int arg = cpu->regs[cpu->bytecode[cpu->ip + 2]];
                    int reg = cpu->bytecode[cpu->ip+1];
                    cpu->ram[cpu->regs[reg]] = arg;

                    cpu->ip += 3;
                }
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: cannot"
                                    " interpret command\n");
                abort();
            }
        }
        else
        {
            if(first_operand_atr & REG)
            {
                int arg = cpu->regs[cpu->bytecode[cpu->ip + 2]];
                int reg = cpu->bytecode[cpu->ip+1];
                cpu->regs[reg] = arg;

                cpu->ip += 3;
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: wrong"
                                    " command format\n");
                abort();
            }
        }
    }
)

// the second operand of move is ram i. e. [register]
CPU_COMMAND(move_ram_reg, 0b01011, 2,
    {
        char first_operand_atr = cpu->bytecode[cpu->ip] & 0xe0;

        if(first_operand_atr & RAM)
        {
            if(first_operand_atr & REG)
            {
                if(first_operand_atr & CONST)
                {
                    int arg = cpu->ram[cpu->regs[cpu->bytecode[cpu->ip + 3]]];
                    int reg = cpu->bytecode[cpu->ip+1];
                    int imm = cpu->bytecode[cpu->ip+2];
                    cpu->ram[cpu->regs[reg] + imm] = arg;

                    cpu->ip += 4;
                }
                else
                {
                    int arg = cpu->ram[cpu->regs[cpu->bytecode[cpu->ip + 2]]];
                    int reg = cpu->bytecode[cpu->ip+1];
                    cpu->ram[cpu->regs[reg]] = arg;

                    cpu->ip += 3;
                }
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: cannot"
                                    " interpret command\n");
                abort();
            }
        }
        else
        {
            if(first_operand_atr & REG)
            {
                int arg = cpu->ram[cpu->regs[cpu->bytecode[cpu->ip + 2]]];
                int reg = cpu->bytecode[cpu->ip+1];
                cpu->regs[reg] = arg;

                cpu->ip += 3;
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: wrong"
                                    " command format\n");
                abort();
            }
        }
    }
)

// the second operand of move is ram i. e. [imm]
CPU_COMMAND(move_ram_imm, 0b01110, 2,
    {
        char first_operand_atr = cpu->bytecode[cpu->ip] & 0xe0;

        if(first_operand_atr & RAM)
        {
            if(first_operand_atr & REG)
            {
                if(first_operand_atr & CONST)
                {
                    int arg = cpu->ram[cpu->bytecode[cpu->ip + 3]];
                    int reg = cpu->bytecode[cpu->ip+1];
                    int imm = cpu->bytecode[cpu->ip+2];
                    cpu->ram[cpu->regs[reg] + imm] = arg;

                    cpu->ip += 4;
                }
                else
                {
                    int arg = cpu->ram[cpu->bytecode[cpu->ip + 2]];
                    int reg = cpu->bytecode[cpu->ip+1];
                    cpu->ram[cpu->regs[reg]] = arg;

                    cpu->ip += 3;
                }
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: cannot"
                                    " interpret command\n");
                abort();
            }
        }
        else
        {
            if(first_operand_atr & REG)
            {
                int arg = cpu->ram[cpu->bytecode[cpu->ip + 2]];
                int reg = cpu->bytecode[cpu->ip+1];
                cpu->regs[reg] = arg;

                cpu->ip += 3;
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: wrong"
                                    " command format\n");
                abort();
            }
        }
    }
)

// the second operand of move is ram i. e. [register + imm]
CPU_COMMAND(move_ram_reg_imm, 0b01111, 2,
    {
        char first_operand_atr = cpu->bytecode[cpu->ip] & 0xe0;

        if(first_operand_atr & RAM)
        {
            if(first_operand_atr & REG)
            {
                if(first_operand_atr & CONST)
                {
                    int arg = cpu->ram[cpu->regs[cpu->bytecode[cpu->ip + 3]] + 
                                       cpu->bytecode[cpu->ip + 4]];
                    int reg = cpu->bytecode[cpu->ip+1];
                    int imm = cpu->bytecode[cpu->ip+2];
                    cpu->ram[cpu->regs[reg] + imm] = arg;

                    cpu->ip += 5;
                }
                else
                {
                    int arg = cpu->ram[cpu->regs[cpu->bytecode[cpu->ip + 2]] +
                                       cpu->bytecode[cpu->ip + 3]];
                    int reg = cpu->bytecode[cpu->ip+1];
                    cpu->ram[cpu->regs[reg]] = arg;

                    cpu->ip += 4;
                }
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: cannot"
                                    " interpret command\n");
                abort();
            }
        }
        else
        {
            if(first_operand_atr & REG)
            {
                int arg = cpu->ram[cpu->regs[cpu->bytecode[cpu->ip + 2]] + 
                                   cpu->bytecode[cpu->ip + 3]];
                int reg = cpu->bytecode[cpu->ip+1];
                cpu->regs[reg] = arg;

                cpu->ip += 4;
            }
            else
            {
                pr_err(LOG_CONSOLE, "Runtime error: wrong"
                                    " command format\n");
                abort();
            }
        }
    }
)

CPU_COMMAND(jmp, 0b10000, 0,
	{
        cpu->ip = cpu->bytecode[cpu->ip + 1];
        //pr_info(LOG_CONSOLE, "ip was %d; change to %d\n", cpu->ip, 
                            //cpu->bytecode[cpu->ip+1]);
	}
)

CPU_COMMAND(je, 0b10001, 0,
	{
        int a = POP(int, stk);
        int b = POP(int, stk);
        if(a == b)
            cpu->ip = cpu->bytecode[cpu->ip + 1];
	}
)

CPU_COMMAND(jne, 0b10010, 0,
	{
        int a = POP(int, stk);
        int b = POP(int, stk);
        if(a != b)
            cpu->ip = cpu->bytecode[cpu->ip + 1];
	}
)

CPU_COMMAND(jg, 0b10011, 0,
	{
        int b = POP(int, stk);
        int a = POP(int, stk);
        if(a > b)
            cpu->ip = cpu->bytecode[cpu->ip + 1];
	}
)

CPU_COMMAND(jge, 0b10100, 0,
	{
        int b = POP(int, stk);
        int a = POP(int, stk);
        if(a >= b)
            cpu->ip = cpu->bytecode[cpu->ip + 1];
	}
)

CPU_COMMAND(jl, 0b10101, 0,
	{
        int b = POP(int, stk);
        int a = POP(int, stk);
        if(a < b)
            cpu->ip = cpu->bytecode[cpu->ip + 1];
	}
)

CPU_COMMAND(jle, 0b10110, 0,
	{
        int b = POP(int, stk);
        int a = POP(int, stk);
        if(a <= b)
            cpu->ip = cpu->bytecode[cpu->ip + 1];
	}
)

// CPU_REG(name, number)

CPU_REG(ax, 0b1)
CPU_REG(bx, 0b10)
CPU_REG(cx, 0b100)
