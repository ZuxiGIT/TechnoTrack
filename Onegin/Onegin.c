#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>
#include <assert.h>
#include <ctype.h>	

#define MIN(a, b) (a) < (b) ? (a) : (b)


typedef struct{ unsigned char* start; unsigned char* finish; int length;} Line;
typedef unsigned char uc;


const char INPUT[] = "/home/eugeniya/Technotrack/Onegin/input.txt";


size_t fsize(const char* name)
{
	assert(name != NULL);

	struct stat stbuf;

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
	buff[size] = '\0';

	size_t written_sz = fread(buff, sizeof(unsigned char), size, fp);
	printf("Declared size (sizeof(char)): %zu Written size (sizeof(char)): %zu\n", size, written_sz);

	fclose(fp);

	return buff;
}

int NumOfLines(const unsigned char* text)
{
	assert(text != NULL);

	int i = 0;
	
	while (*text != '\0')
	{
		if (*text == '\n')
			i++;
		text++;
	}
	i++;
	return i;
}

Line* ParseLines(unsigned char* lines, int size)
{
	assert(lines != NULL);

	Line* index = (Line*)calloc(size, sizeof(Line));
	
	unsigned char* begin = lines;
	unsigned char* end   = begin;

	for (int i = 0; i < size; i ++)
	{
		while (*end != '\n' && *end != '\0')
			end++;
		
		*end = '\0';

		index[i].start 	= begin;
		index[i].finish = end;
		index[i].length = (end - begin);

		begin = end + 1;
		end = begin;
	
	}

	//printf("\n---------------------------\n");
	
	//for ( int k = 0; k < size; k ++)
	//	printf("i = %d start = %p finish = %p\nstring = %s\n", k, index[k].start, index[k].finish, index[k].start);
	
	//for ( int j = 0; j < size; j++)
	//	printf("j = %d start from %c end with %c length is %d\n%s\n", j, *index[j].start, *index[j].finish, index[j].length, index[j].start);

	return index;
}

int isletter(unsigned char symbol)
{

	return ((uc)'à' <= symbol && symbol <= (uc)'ÿ') || ((uc)'À' <= symbol && symbol <= (uc)'ß');
	//printf("\n--%hhu--(%c code: %d)----(res is %d)\n",'à', symbol, symbol, res);
}	


int DirectComparisonForLines(const void* a, const void* b)
{
	assert(a != NULL);
	assert(b != NULL);

	printf("\nCompare two lines:\n\t%s\n\t%s\n", ((const Line*)a)->start, ((const Line*)b)->start);
	
	const Line fst  = *(const Line*)a;
	const Line snd  = *(const Line*)b;

	
	int i = 0;
	int j = 0;

	while(i < fst.length && j < snd.length)
	{
		if (isletter(fst.start[i])) ; else { i++; continue;}
		if (isletter(snd.start[j])) ; else { j++; continue;}

		if(fst.start[i] > snd.start[j]) {printf("\nres: 1 > 2 (%c > %c)", fst.start[i], snd.start[j]); return  1;}
		if(fst.start[i] < snd.start[j]) {printf("\nres: 1 < 2 (%c < %c)", fst.start[i], snd.start[j]); return -1;}

		i++;
		j++;
	}
	if (i < j) return -1;
	if (i > j) return  1;
	return 0;
}


int ReverseComparisonForLines(const void* a, const void* b)
{
	assert(a != NULL);
	assert(b != NULL);

	printf("\nCompare two lines:\n\t%s\n\t%s\n", ((const Line*)a)->start, ((const Line*)b)->start);

	const Line fst  = *(const Line*)a;
	const Line snd  = *(const Line*)b;

	int i = 1;
	int j = 1;

	while(i <= fst.length && j <= snd.length)
	{
		if (isletter(fst.finish[-i])) ; else { i++; continue;}
		if (isletter(snd.finish[-j])) ; else { j++; continue;}

		if(fst.finish[-i] > snd.finish[-j]) {printf("\nres: 1 > 2 (%c > %c)", fst.finish[-i], snd.finish[-j]); return  1;}
		if(fst.finish[-i] < snd.finish[-j]) {printf("\nres: 1 < 2 (%c < %c)", fst.finish[-i], snd.finish[-j]); return -1;}

		i++;
		j++;
	}

	if (i < j) return -1;
	if (i > j) return  1;
	return 0;

}


int main()
{

	char* current_locale = setlocale(LC_ALL, "");
	setlocale(LC_ALL, "ru_RU.CP1251");  //ru_RU.CP1251

	size_t sz = fsize(INPUT);
	printf("sz is %zu\n", sz);

	char* txt = CreateText(INPUT, sz);
	int num_of_lines = NumOfLines(txt);

	printf("\nTotal number of lines is %d\n", num_of_lines);
	
	printf("read :\n%s\n", txt);


	Line* ind = ParseLines(txt, num_of_lines);

	qsort(ind, num_of_lines, sizeof(Line), DirectComparisonForLines);
	qsort(ind, num_of_lines, sizeof(Line), ReverseComparisonForLines);
	for ( int j = 0; j < num_of_lines; j++)
		printf("j = %d start from %c end with %c length is %d\n%s\n", j, *ind[j].start, *ind[j].finish, ind[j].length, ind[j].start);


	free(txt);
	setlocale(LC_ALL, current_locale);
	free(current_locale);
}