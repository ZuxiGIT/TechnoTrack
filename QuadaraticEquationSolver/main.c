#include "EqSolver.h"
#include <assert.h>
#include <stdio.h>
#include <malloc.h>

int main()
{
	// its better to use double array while later i use 
	// construction like scanf("%lf %lf %lf", coefficients_array, coefficients_array + 1, coefficients_array + 2 ) != 3

	double* coeff_mass	=	(double*)calloc(3, sizeof(double)); //  3 double variables 
	double* roots 		=	(double*)calloc(2, sizeof(double)); //
	Test();
	Input(coeff_mass); // solve?
	
	PrintResult(SquareEqSolver(roots, coeff_mass), roots); // solve then print result

	free(coeff_mass);
	free(roots);

	return 0;
}