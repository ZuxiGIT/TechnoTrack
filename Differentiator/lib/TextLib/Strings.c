#include "Strings.h"
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <memory.h>
#include <wchar.h>
#include <stdio.h>

typedef unsigned char uc;

/*
int isLetter(unsigned char symbol)
{

	return ((uc)'�' <= symbol && symbol <= (uc)'�')  ||\
			((uc)'�' <= symbol && symbol <= (uc)'�') ||\
			isalpha(symbol);
}

unsigned char toLower(unsigned char symbol)
{
	if (symbol < (uc)'�')
		return symbol + ((uc)'�' - (uc)'�');
	else 
		return symbol;
}

*/
int numberOfLines(const wchar_t* text)
{
	assert(text != NULL);

	int i = 0;

    const wchar_t* beginning = text;

    freopen(NULL, "w", stdout);

	while (*text != '\0')
	{
		if (*text == '\n')
			i++;
		text++;
	}

    freopen(NULL, "w", stdout);
    
    //why i wrote this before?????????
	if (beginning != text)
		i++;
		
	return i;
}

Line* parseText(wchar_t* lines, int size)
{
	assert(lines != NULL);

	Line* index = (Line*)calloc(size, sizeof(Line));

	assert(index != NULL);
	
	wchar_t* begin = lines;
	wchar_t* end   = begin;

	for (int i = 0; i < size; i ++)
	{
		while (*end != L'\n' && *end != L'\0') // strchr strtok
			end++;
		
		*end = L'\0';


		index[i].length = (end - begin);
		index[i].start 	= begin; //wcsdup(begin); 
		index[i].finish = end; //index[i].start + index[i].length;
		// memcpy(index[i].start, begin, index[i].length);
		// index[i].finish = index[i].start + begin;

		begin = end + 1;
		end = begin;	
	}

	return index;
}

