#define DEBUG
#define PROTECT

#define StkElem int
#include "Stack.hpp"







int main(int argc, char const *argv[])
{
	Stack stk = {};
	StackCtor (&stk, 13);
	return 0;
}