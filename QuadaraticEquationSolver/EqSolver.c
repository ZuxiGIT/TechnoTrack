#include "EqSolver.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

static const double precision = 1e-5;

static char _DoubleZeroCompare(double num);
static char _SquareEqSolver(double*, double, double , double);
static void _ClearInputBuff();


char SquareEqSolver(double* roots, double* coeff_mass)
{
	return _SquareEqSolver(roots, coeff_mass[0], coeff_mass[1], coeff_mass[2]);
}

char CompareDoubles(double num1, double num2)
{
	if(fabs(num1 - num2) < precision)
		return 1;
	else
		return 0;
}

char DoubleZeroCompare(double num)
{
	return _DoubleZeroCompare(num);
}

char LinearEqSolver(double* roots, double a, double b)
{
	assert(roots);

	double* x1 = roots;
	double* x2 = roots + 1;

	*x1 = NAN;
	*x2 = NAN;

	if(DoubleZeroCompare(a)) 
	{
		if(DoubleZeroCompare(b))
			return INF;
		else
			return NOROOTS;
	}
	else
	{
		*x1 = -b / a;
		
		if(CompareDoubles(*x1, 0.))
		{
			*x1 = 0;
		}
		
		return 1;
	}	
}


void Input(double* coeff_mass)
{
	assert(coeff_mass);


// scanf("%f %f %f") == 3
	printf("Enter the coefficients, plz\n");	
	while(scanf("%lf %lf %lf", coeff_mass, coeff_mass + 1, coeff_mass + 2 ) != 3)
	{
		printf("Wrong number, try again (may be you use \",\" instead \".\"\n");

		_ClearInputBuff();
	}

}

void PrintResult(int result, double* roots)
{

	switch(result)
	{
		case INF:
			printf("This eq has infinity roots\n");
			break;

		case NOROOTS:
			printf("This eq has no roots\n");
			break;

		case ONE_ROOT:
			printf("This eq has 1 root %lg\n", roots[0]);
			break;

		case TWO_ROOTS:
			printf("This eq has 2 roots %lg and %lg\n", roots[0], roots[1]);
			break;

	}
}

char _DoubleZeroCompare(double num)
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

char _SquareEqSolver(double* roots, double a, double b, double c)
{
	assert(roots);

	double* x1 = roots;
	double* x2 = roots + 1;
	
	if(DoubleZeroCompare(a))
		return LinearEqSolver(roots, b, c);

	if(DoubleZeroCompare(b))  
	{	
		
		if(DoubleZeroCompare(c)) 
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

	if  (discr < 0)
		return NOROOTS;

	double sqrt_disc = sqrtf(discr);

	*x1 = (-b + sqrt_disc)/(2*a);
	*x2 = (-b - sqrt_disc)/(2*a);
	
	if (*x1 == *x2)
		return ONE_ROOT;

	return TWO_ROOTS; 
}

void _ClearInputBuff()
{
	char buff_char = '0';

	while((buff_char = getchar()) != '\n' || buff_char != EOF );
}