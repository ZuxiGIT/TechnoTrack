#include "Comparisons.h"
#include "Strings.h"
#include "Sort.h"
#include "logger.h"
#include "Text.h"
#include "File.h"
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char* argv[])
{

	//const char* PROGRAM_NAME = argv[0];
	if ( argc == 1 )
	{
		pr_err( LOG_CONSOLE | LOG_FILE, "no input file\n");
		return 1;
	} // parse_args

	const char* input_file = argv[1]; 



	Text* text = text_init(input_file);
	filePrint(text); // struct Text*
	
	selectionSort(text, DirectComparisonForLines);
	
	filePrint(text);

	generalizedSelectionSort(text->text, text->num_of_lines, sizeof(Line), DirectComparisonForLines);

	filePrint(text);

	
	qsort(text->text, text->num_of_lines, sizeof(Line), ReverseComparisonForLines);
	
	filePrint(text);
	
	//for ( int j = 0; j < num_of_lines; j++)
		//printf("j = %d start from %c end with %c length is %d\n%s\n", j, *ind[j].start, *ind[j].finish, ind[j].length, ind[j].start);
	
	for (int i = 0; i < text->num_of_lines; i++)
		printf("%s\n", text->text[i].start); // print_lines

	
	log_close();
	text_free(text);
	free(text);
	
	return 0;
}