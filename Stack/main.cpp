#define DEBUG
#define PROTECT

#define StkElem int
#include "Stack.hpp"



#define NAME_OF_ARG(ARG)	#ARG



int main(int argc, char const *argv[])
{
	STACK(int) stk = {};
	CTOR(int, stk, 13);


	CheckStack("Checking", NAME_OF_ARG(stk), &stk);
	


	return 0;
}