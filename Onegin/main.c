#include "Comparisons.h"
#include "Strings.h"
#include "Sort.h"
#include "File.h"
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{

	//const char* PROGRAM_NAME = argv[0];
	if ( argc == 1)
	{
		printf("ERROR: enter the input file\n");
		return 1;
	}

	const char* INPUT = argv[1];

	char* current_locale = setlocale(LC_ALL, "");
	setlocale(LC_ALL, "ru_RU.CP1251"); 

	size_t sz = fileSize(INPUT);
	printf("sz is %zu\n", sz);

	unsigned char* txt = readText(INPUT, sz);
	int num_of_lines = numberOfLines(txt);

	printf("\nTotal number of lines is %d\n", num_of_lines);
	
	printf("read :\n%s\n", txt);
	printf("\n\n\n");


	Line* ind = parseText(txt, num_of_lines);
	
	filePrint(ind, num_of_lines);
	
	selectionSort(ind, num_of_lines, DirectComparisonForLines);
	
	filePrint(ind, num_of_lines);
	
	qsort(ind, num_of_lines, sizeof(Line), ReverseComparisonForLines);
	
	filePrint(ind, num_of_lines);
	
	//for ( int j = 0; j < num_of_lines; j++)
		//printf("j = %d start from %c end with %c length is %d\n%s\n", j, *ind[j].start, *ind[j].finish, ind[j].length, ind[j].start);
	
	for (int i = 0; i < num_of_lines; i++)
		printf("%s\n", ind[i].start);

	setlocale(LC_ALL, current_locale);
	
	free(ind);
	free(txt);
	free(current_locale);
	
	return 0;
}