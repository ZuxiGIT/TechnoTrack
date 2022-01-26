#include "assembler.h"
#include "../lib/TextLib/File.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
	Text* input = text_init(argv[1]);
	printf("[Line:%d] File: %s: CHECK\n", __LINE__, strrchr(__FILE__, '/') + 1);
	Text* output = compilation(input);
	printf("[Line:%d] File: %s: CHECK\n", __LINE__, strrchr(__FILE__, '/') + 1);
	fprintTextBinary(output);
	printf("[Line:%d] File: %s: CHECK\n", __LINE__, strrchr(__FILE__, '/') + 1);
	text_free(input);
	text_free(output);
    // printf("isalpha(\\0) = %d\nisalpha(\'0\') = %d\n", isalpha('\0'), isalpha('0'));
	// printf("isspace(\'\\0\'a) = %d", isspace('\0'));
    
	return 0;
}
