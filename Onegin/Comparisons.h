#ifndef Comparisons_hpp
#define Comparisons_hpp
/*! \file */

/** 
	Function that can compare two Line variables (direct)

	\param[in] a - void pointer to the Line variable
	\param[in] b - void pointer to the Line variable

	\return 1 if *a > *b else 0
*/

int DirectComparisonForLines(const void* a, const void* b);

/** 
	Function that can compare two Line variables (reverse)

	\param[in] a - void pointer to the Line variable
	\param[in] b - void pointer to the Line variable

	\return 1 if *a > *b else 0
*/

int ReverseComparisonForLines(const void* a, const void* b);

#endif /* Comparisons_hpp */