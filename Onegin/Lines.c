#include "Lines.h"


int isletter(unsigned char symbol)
{

	return ((uc)'à' <= symbol && symbol <= (uc)'ÿ') || ((uc)'À' <= symbol && symbol <= (uc)'ß');
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

	assert(index != NULL);
	
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

	return index;
}

