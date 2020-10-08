#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "EqSolver.h"




void Input(double* coeff_mass)
{
	assert(coeff_mass);

	char buff_char = '0';

	for(int i = 0; i < 3; i ++)
	{
		printf("Enter the %d coeff, plz\n", i + 1);	
		while(scanf("%lg", (coeff_mass + i)) == 0 || (buff_char = getchar()) == ',')
		{
			printf("Wrong number, try again\n");
			if(buff_char == ',')
				printf("Plz use point instead comma\n");

			while((buff_char = getchar()) != '\n');   // try to empty input buffer
		}
		buff_char = '0';
	}
}


void PrintResult(double* roots, double* coeff_mass)
{
	assert(roots);
	assert(coeff_mass);

	switch(SquareEqSolver(roots, coeff_mass[0], coeff_mass[1], coeff_mass[2]))
	{
		case INF:
			printf("This eq has infinity roots\n");
			break;
		case NOROOTS:
			printf("This eq has no roots\n");
			break;
		case 1:
			printf("This eq has 1 root %lg\n", roots[0]);
			break;
		case 2:
			printf("This eq has 2 roots %lg and %lg\n", roots[0], roots[1]);
			break;
	}
}


int main()
{

	double* coeff_mass	=	(double*)calloc(3, sizeof(double));
	double* roots 		=	(double*)calloc(2, sizeof(double));
	Input(coeff_mass);
	PrintResult(roots, coeff_mass);

	free(coeff_mass);
	free(roots);
	return 0;
}