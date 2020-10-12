#include "Lines.h"

/**
	Checks if the symbol is letter

	\param[in] symbol - unsigned char 

	\return 1 if the symbol is letter and 0 if not
*/
int isletter(unsigned char symbol)
{

	return ((uc)'à' <= symbol && symbol <= (uc)'ÿ') || ((uc)'À' <= symbol && symbol <= (uc)'ß') \
		|| isalpha(symbol);
}

/**
	Convert uppercase to lowercase

	\param[in] symbol - unsigned char

	\return lowercase
*/

unsigned char toLower(unsigned char symbol)
{
	if (symbol < (uc)'à')
		return symbol + ((uc)'à' - (uc)'À');
	else 
		return symbol;
}

/** 
	Count lines

	\param[in] text - pointer to the array of unsigned char

	\return number of lines
*/
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

/**
	Parse lines from the array of unsigned char

	\param[in] text - pointer to the array of unsigned char
	\param[in] size - number of lines

	\return pointer to the array of Line
*/

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

