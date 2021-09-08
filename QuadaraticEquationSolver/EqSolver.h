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
char SquareEqSolver(double*, double, double, double);


enum RESULT
{
	NOROOTS = -2,
	INF = -1,
	ONE_ROOT = 1,
	TWO_ROOTS = 2
};

#endif