#ifndef SORT_H
#define SORT_H
#include "Text.h"

/*! \file */

/**
	\brief Selection Sort

	\param[in] ind   - pointer to the array of Line 
	\param[in] size  - number of lines
	\param[in] *comp - function, that can compare two Line variables
*/


void selectionSort(Text* text, int (*comp)(const void* a, const void* b));

/**
	\brief Selection Sort

	\param[in] buff   - pointer to the array of elemnts 
	\param[in] size  - number of elements
	\param[in] elem_size  - size of element
	\param[in] *comp - function, that can compare two elements variables
*/
void generalizedSelectionSort(void* buff, int size, int elem_size, int(*comp)(const void* a, const void* b));

// /** 
// 	Swap two Line variables

// 	\param[in] a - pointer to the first Line variable
// 	\param[in] b - pointer to the second Line variable
// */
// void swap(Line* a, Line* b);

#endif /* SORT_H */