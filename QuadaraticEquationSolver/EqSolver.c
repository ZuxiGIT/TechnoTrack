#include "EqSolver.h"
#include <math.h>
#include <assert.h>


static const double precision = 1e-5;

/**

	\brief Compare two double-precision floating-point numbers with certain accuracy
	
	\param[in] num1	- First number
	\param[in] num2	- Second number

	\return if(num1 == num2) return 1 else return  0
*/

char Compare_Doubles(double num1, double num2)
{
	if(fabs(num1 - num2) < precision)
		return 1;
	else
		return 0;
}

/**

	Compare double-precision floating-point number with zero using IEEE 754-2008 standard 

	\param[in] num  - Double-precision floating-point number 

	\return if (num == 0) 1 else 0
*/

char double_zero_compare(double num)
{
	union
	{
		double doub;
		long long ll;
	} exp;

	exp.doub = num;

	int res = (exp.ll & (long long) 0xfff << 52 ) >> 52;
	if (res == 0x0 || res == 0x8000)
		return 1;

	return 0;
}




char LinearEqSolver(double* roots, double a, double b)
{
	assert(roots);

	//printf("It is a Linear eq.");
	double* x1 = roots;
	double* x2 = roots + 1;

	*x2 = NAN;

	if(double_zero_compare(a))
		if(double_zero_compare(b))
			return INF;
		else
			return NOROOTS;
	else
	{
		*x1 = -b / a;
		if(Compare_Doubles(*x1, 0.))
			*x1 = 0;
		return 1;
	}	
}



char SquareEqSolver(double* roots, double a, double b, double c)
{
	assert(roots);

	double* x1 = roots;
	double* x2 = roots + 1;
	
	if(double_zero_compare(a))
		return LinearEqSolver(roots, b, c);

	if(double_zero_compare(b))  
	{	
		if(double_zero_compare(c)) 
		{
			*x1 = *x2 = 0;
			return 1;
		}
		if (a*c < 0)
		{
			*x1 = - c / a;
			*x1 = - *x2;
			return 2;
		}
		return NOROOTS;
	}
	
	double discr = b*b - 4*a*c;

	if(discr < 0)
		return NOROOTS;

	double sqrt_disc = sqrtf(discr);
	*x1 = (-b + sqrt_disc)/(2*a);
	*x2 = (-b - sqrt_disc)/(2*a);
	
	if(*x1 == *x2)
		return 1;

	return 2;
}