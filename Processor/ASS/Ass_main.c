#include <stdio.h>
#include "assembler.h"
#include <ctype.h>

int main(int argc, char const *argv[])
{

	compilation(text_init(argv[1]));
    // printf("isalpha(\\0) = %d\nisalpha(\'0\') = %d\n", isalpha('\0'), isalpha('0'));
	// printf("isspace(\'\\0\'a) = %d", isspace('\0'));
    
	return 0;
}