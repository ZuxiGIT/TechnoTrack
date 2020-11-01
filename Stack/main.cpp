#define DEBUG
#define PROTECT

#define StkElem int
#include "Stack.hpp"







int main(int argc, char const *argv[])
{
	STACK(int) stk = {};
	CTOR(int, stk, 13);
	//CTOR(int, stk, 19);
	StkElem val = POP(int, stk);
	Print(stdout,  val);
	CheckStack("Checking", NAME_OF_ARG(stk), &stk)
	


	return 0;
}