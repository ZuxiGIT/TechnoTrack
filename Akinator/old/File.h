#ifndef FILE_H
#define FILE_H
#include <stddef.h>
#include "Text.h"
/*! \file */

/**
	\brief Calculate the file's size

	\param[in] name - the file name

	\return the file's size
*/

size_t fileSize(const char* name);

/**
	\brief Writes the array of Lines to a file

	\param[in] input  - struct Text
	
*/

void filePrint(Text* input);

/**
	@brief Read text from the file

	@param[in] name - the filename
	@param[in] size - the file's size

	@return pointer to the array of unsigned char
*/

unsigned char* readText(const char* name, const size_t size);

#endif /* FILE_H */