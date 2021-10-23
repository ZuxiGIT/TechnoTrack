#include <stdio.h>
#include "assembler.h"
#include <ctype.h>

int main(int argc, char const *argv[])
{

	compilation(text_init(argv[1]));
    //printf("isalpha(\'c\') = %d\nisalpha(\'0\') = %d\n", isalpha('c'), isalpha('0'));
    
	return 0;
}