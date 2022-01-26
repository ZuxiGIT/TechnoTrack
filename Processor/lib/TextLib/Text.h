#ifndef TEXT_H
#define TEXT_H
#include "Strings.h"
#include <stddef.h>

/*! \file */


typedef struct
{
    Line* text;
    int num_of_lines;
} Text;

void text_free(Text* text);

Text* text_init(const char* path);

/**
	\brief Writes the array of Lines to a file

	\param[in] input  - struct Text
	
*/

void fprintText(Text* input);

void fprintTextBinary(Text* input);

void fprintTextWithDelimeters(Text* input, char delim);

void printText(Text* src);

void printTextWithDelimeters(Text* src, char delim);

int textBinaryLength(Text* txt);


#endif /* TEXT_H */
