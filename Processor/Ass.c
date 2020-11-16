#include "FText.h"


int main(int argc, char const *argv[])
{
	if (argc == 1)
	{
		printf("ERROR: Not input file, enter the filename\n");
		return 1;
	}

	int error_code;

	const char* FILE_NAME 	= argv[1];

	size_t sz_of_file 		= fsize(FILE_NAME);

	char* txt 				= CreateText(FILE_NAME, sz_of_file);
	size_t num_of_lines 	= NumOfLines(txt);

	Line* ind 				= ParseLines(txt, num_of_lines); 
	int* output 			= Translate(ind, num_of_lines, &error_code);
	
	FPrint(ind, num_of_lines);


	return 0;	
}