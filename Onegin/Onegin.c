#include "Lines.h"	
#include "Comparisons.h"
#include "Sort.h"


size_t fsize(const char* name)
{
	assert(name != NULL);

	struct stat stbuf = {};

	if (stat(name, &stbuf) == -1)
	{	
		fprintf(stderr, "fsize: can't access %s\n", name);
		return 0;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		fprintf(stderr, "fsize: can't access %s. It is a directory", name);
		return 0;
	}
	return stbuf.st_size;
}


unsigned char* CreateText(const char* name, const size_t size)
{
	assert(name != NULL);

	FILE* fp = fopen(name, "r");
	
	assert(fp != NULL);
	
	unsigned char* buff = (unsigned char*)calloc(size + 1, sizeof(unsigned char));
	
	assert(buff != NULL);

	buff[size] = '\0';

	size_t written_sz = fread(buff, sizeof(unsigned char), size, fp);
	printf("Declared size (sizeof(char)): %zu Written size (sizeof(char)): %zu\n", size, written_sz);

	fclose(fp);

	return buff;
}


void FPrint(Line* ind, int num_of_lines)
{
	FILE* fp = fopen("output.txt", "a");
	
	assert(fp != NULL);

	for (int i  = 0; i < num_of_lines; i ++)
		fprintf(fp, "%s\n", ind[i].start);
	fprintf(fp, "\n============================================\n"
				"\n============================================\n");

	fclose(fp); 
}

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

	size_t sz = fsize(INPUT);
	printf("sz is %zu\n", sz);

	unsigned char* txt = CreateText(INPUT, sz);
	int num_of_lines = NumOfLines(txt);

	printf("\nTotal number of lines is %d\n", num_of_lines);
	
	printf("read :\n%s\n", txt);
	printf("\n\n\n");


	Line* ind = ParseLines(txt, num_of_lines);
	FPrint(ind, num_of_lines);
	SelectionSort(ind, num_of_lines, DirectComparisonForLines);
	FPrint(ind, num_of_lines);
	qsort(ind, num_of_lines, sizeof(Line), ReverseComparisonForLines);
	FPrint(ind, num_of_lines);
	//for ( int j = 0; j < num_of_lines; j++)
		//printf("j = %d start from %c end with %c length is %d\n%s\n", j, *ind[j].start, *ind[j].finish, ind[j].length, ind[j].start);
	for (int i = 0; i < num_of_lines; i++)
		printf("%s\n", ind[i].start);

	free(ind);
	free(txt);
	setlocale(LC_ALL, current_locale);
	free(current_locale);
	return 0;
}