#include "Test.h"
#include "EqSolver.h"
#include <malloc.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLACK "\033[37m"
#define RESET "\033[0m"


struct test_frame
{
    double a;
    double b;
    double c;
    enum RESULT number_of_root;
    double root1;
    double root2;
};

static const struct test_frame test_array[] = 
{
    {1,  2, 1, ONE_ROOT, -1},
    {1, -2, 1, ONE_ROOT,  1},
};


const char* _res2str(enum RESULT res)
{
	switch(res)
	{
		case INF:
			return "INF";

		case NOROOTS:
            return "NOROOTS";

		case ONE_ROOT:
            return "ONE_ROOT";

		case TWO_ROOTS:
            return "TWO_ROOTS";

	}
}



void Test()
{
    double* roots = (double*)calloc(2, sizeof(double));  

    for(int i = 0; i < sizeof(test_array) / sizeof(test_array[0]); i++)
    {
        double* coeff_mass = &test_array[i];

        int result = SquareEqSolver(roots, coeff_mass);
        
        if (result != test_array[i].number_of_root)
            printf(RED "TEST %i FAILED: expected number of roots %s, got %s\n"
            "\tInput coeff a = %.2lf, b = %.2lf, c = %.2lf\n"BLACK,\
            i, _res2str(test_array[i].number_of_root), _res2str(result), test_array[i].a, test_array[i].b, test_array[i].c);

        switch (result)
        {
            case ONE_ROOT:
                if (*roots != test_array[i].root1)
                {

                    printf(RED"TEST %i FAILED: expected root %.2lf, got %.2lf\n"
                    "\tInput coeff a = %.2lf, b = %.2lf, c = %.2lf\n"RESET,\
                    i, test_array[i].root1, *roots, test_array[i].a, test_array[i].b, test_array[i].c);
                    
                    break;
                }
            
            case TWO_ROOTS:
                if ((*roots != test_array[i].root1 && *roots != test_array[i].root2) ||
                    (*(roots + 1) != test_array[i].root1 && *(roots + 1) != test_array[i].root2))
                {
                    printf(RED"TEST %i FAILED: expected root %.2lf, got %.2lf\n"
                    "\tInput coeff a = %.2lf, b = %.2lf, c = %.2lf\n"RESET,\
                    i, test_array[i].root1, *roots, test_array[i].a, test_array[i].b, test_array[i].c);
                    
                    break;
                }
            
            case INF:
            case NOROOTS:
                printf(GREEN"TEST %i PASSED\n"RESET, i);
        }
    }
}