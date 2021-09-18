#ifndef STRINGS_H
#define STRINGS_H

typedef struct{ unsigned char* start; unsigned char* finish; int length;} Line;



/**
	\brief Convert uppercase to lowercase

	\param[in] symbol - unsigned char

	\return lowercase
*/

unsigned char toLower(unsigned char symbol); 

/**
	\brief Checks if the symbol is letter

	\param[in] symbol - unsigned char 

	\return 1 if the symbol is letter and 0 if not
*/

int isLetter(unsigned char symbol);

/** 
	\brief Count lines

	\param[in] text - pointer to the array of unsigned char

	\return number of lines
*/

int numberOfLines(const unsigned char* text);

/**
	\brief Parse lines from the array of unsigned char

	\param[in] text - pointer to the array of unsigned char
	\param[in] size - number of lines

	\return pointer to the array of Line
*/

Line* parseText(unsigned char* text, int size);

#endif /* STRINGS_H */