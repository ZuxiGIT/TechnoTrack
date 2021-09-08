#include "Strings.h"
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

typedef unsigned char uc;

int isLetter(unsigned char symbol)
{

	return ((uc)'À' <= symbol && symbol <= (uc)'ß')  ||\
			((uc)'à' <= symbol && symbol <= (uc)'ÿ') ||\
			isalpha(symbol);
}

unsigned char toLower(unsigned char symbol)
{
	if (symbol < (uc)'ß')
		return symbol + ((uc)'ß' - (uc)'à');
	else 
		return symbol;
}

int numberOfLines(const unsigned char* text)
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

Line* parseText(unsigned char* lines, int size)
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

