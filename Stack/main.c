#define StkElem int
#include "Stack.h"

#define StkElem double
#include "Stack.h"




int main()
{
	STACK(int) stk = {};	
	CTOR(int, stk, 2);
	CTOR(int, stk, 2);
	PUSH(int, stk, 10);
	PUSH(int, stk, 20);
	PUSH(int, stk, 30);
	PUSH(int, stk, 40);
	PUSH(int, stk, 50);


	STACK(double) stk_d = {};
	CTOR(double, stk_d, 2);
	PUSH(double, stk_d, 1.7);
	PUSH(double, stk_d, 2.2);
	PUSH(double, stk_d, 3.3);
	PUSH(double, stk_d, 4.5);
	PUSH(double, stk_d, 5.6);


	int val = POP(int, stk);
	// printf("first value is ");
	// Print(stdout,  val);
	val = POP(int, stk);
	// printf("second value is ");
	// Print(stdout,  val);
	printf("line: %d\n", __LINE__);

	val = POP(int, stk);
	printf("line: %d\n", __LINE__);

	// printf("third value is ");
	// Print(stdout,  val);
	// printf("checking--------\n");
	printf("line: %d\n", __LINE__);
	
	checkStack(int, "Checking", &stk);
	printf("line: %d\n", __LINE__);
	DTOR(int, stk);
	

	// printf("line: %d\n", __LINE__);
	// double val1 = POP(double, stk_d);
	// printf("line: %d\n", __LINE__);

	// // printf("first value is ");
	// // Print(stdout,  val1);
	// val1 = POP(double, stk_d);
	// // printf("second value is ");
	// // Print(stdout,  val1);
	// printf("line: %d\n", __LINE__);

	// val1 = POP(double, stk_d);
	// printf("line: %d\n", __LINE__);

	// // printf("third value is ");
	// // Print(stdout,  val1);
	// // printf("checking--------\n");
	// CheckStack(double, "Checking", NAME_OF_ARG(stk_d), &stk_d);
	// DTOR(double, stk_d);
	printf("\n%s\n", __FUNCTION__);

	return 0;
}