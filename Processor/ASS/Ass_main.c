#include <stdio.h>
#include "assembler.h"
#include <ctype.h>

int main(int argc, char const *argv[])
{
	Text* input = text_init(argv[1]);
	compilation(input);
	text_free(input);
    // printf("isalpha(\\0) = %d\nisalpha(\'0\') = %d\n", isalpha('\0'), isalpha('0'));
	// printf("isspace(\'\\0\'a) = %d", isspace('\0'));
    
	return 0;
}