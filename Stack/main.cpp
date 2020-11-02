#define DEBUG
#define PROTECT

#define StkElem int
#include "Stack.hpp"







int main(int argc, char const *argv[])
{
	STACK(int) stk = {};
	CTOR(int, stk, 2);
	PUSH(int, stk, 10);
	PUSH(int, stk, 20);
	StkElem val = POP(int, stk);
	printf("first value is ");
	Print(stdout,  val);
	val = POP(int, stk);
	printf("second value is ");
	Print(stdout,  val);
	val = POP(int, stk);
	printf("third value is ");
	Print(stdout,  val);
	printf("checking--------\n");
	CheckStack("Checking", NAME_OF_ARG(stk), &stk)
	


	return 0;
}