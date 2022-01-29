#include "Strings.h"
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <memory.h>

typedef unsigned char uc;

int isLetter(unsigned char symbol)
{

	return ((uc)'À' <= symbol && symbol <= (uc)'ß')  ||\
			((uc)'à' <= symbol && symbol <= (uc)'ÿ') ||\
			isalpha(symbol);
}

unsigned char toLower(unsigned char symbol)
{
	if (symbol < (uc)'à')
		return symbol + ((uc)'à' - (uc)'À');
	else 
		return symbol;
}

int numberOfLines(const unsigned char* text)
{
	assert(text != NULL);

	int i = 0;

	const unsigned char* beginning = text;

	while (*text != '\0')
	{
		if (*text == '\n')
			i++;
		text++;
	}
    
    //why i wrote this?????????
	//if (beginning != text)
		//i++;
		
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
		while (*end != '\n' && *end != '\0') // strchr strtok
			end++;
		
		*end = '\0';


		index[i].length = (end - begin);
		index[i].start 	= strndup(begin, end - begin);
		index[i].finish = index[i].start + index[i].length;
		// memcpy(index[i].start, begin, index[i].length);
		// index[i].finish = index[i].start + begin;

		begin = end + 1;
		end = begin;	
	}

	return index;
}

