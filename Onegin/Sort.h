#ifndef SORT_H
#define SORT_H

/**
	Selection Sort

	\param[in] ind   - pointer to the array of Line 
	\param[in] size  - number of lines
	\param[in] *comp - function, that can compare two Line variables
*/

void selectionSort(Line* ind, int size, int (*comp)(const void* a, const void* b));

// /** 
// 	Swap two Line variables

// 	\param[in] a - pointer to the first Line variable
// 	\param[in] b - pointer to the second Line variable
// */
// void swap(Line* a, Line* b);

#endif /* SORT_H */