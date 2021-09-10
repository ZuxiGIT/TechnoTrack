#ifndef EqSolver_H
#define EqSolver_H

/**
	Solves Linear equation

	\param[in] roots - List of roots, where roots will be written
	\param[in] a 	 - First 	coefficient
	\param[in] b 	 - Second coefficient
		
	\return Number of roots or other constants (INF -1) (NOROOTS -2)
		
*/
char LinearEqSolver(double*, double, double);


/**
	Solves quadratic equation

	\param[in] roots - List of roots, where roots will be written
	\param[in] a 	 - First coefficient
	\param[in] b 	 - Second coefficient
	\param[in] c 	 - Third coefficient
		
	\return Number of roots or other constants (INF -1) (NOROOTS -2)
		
*/
char SquareEqSolver(double*, double*);


/**

	\brief Compare two double-precision floating-point numbers with certain accuracy
	
	\param[in] num1	- First number
	\param[in] num2	- Second number

	\return if(num1 == num2) return 1 else return  0
*/

char CompareDoubles(double num1, double num2);

/**

	Compare double-precision floating-point number with zero using IEEE 754-2008 standard 

	\param[in] num  - Double-precision floating-point number 

	\return if (num == 0) 1 else 0
*/

char DoubleZeroCompare(double num);

/**

	Input coefficients

	\param[in] coefficients_array - pointer to double array size of 3

*/
void Input(double* coeff_mass);

/**

	Prints the results of SquareEqSolver

	\param[in] result - result of SquareEqSolver
	\param[in] roots - pointer to double array of 2 

*/
void PrintResult(int result, double* roots);



enum RESULT
{
	NOROOTS = -2,
	INF = -1,
	ONE_ROOT = 1,
	TWO_ROOTS = 2
};

#endif