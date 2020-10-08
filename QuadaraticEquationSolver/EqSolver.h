extern char LinearEqSolver(double*, double, double);
extern char SquareEqSolver(double*, double, double, double);
extern char double_zero_compare(double);
enum RESULT
{
	INF = -1,
	FALSE = 0,
	TRUE = 1,
	NOROOTS = -2
};