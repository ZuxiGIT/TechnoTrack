#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define STACK_PROTECT


#ifdef STACK_PROTECT
#define ON_STACK_PROTECT(code) code
#else
#define ON_STACK_PROTECT(code)
#endif


#define StackClear(stk) if (stk == stk->pointer) { assert(!"Trying to initialize initialized stack!");}


typedef unsigned long long StkCanary;
typedef int 			   StkElem;

const StkElem 	POSION_VALUE	= 2699;
const StkCanary CanaryValue 	= 0xBEAFDEDDEADF00D;

enum {
	FULLSTACK
};



struct Stack
{
	ON_STACK_PROTECT(StkCanary FrontCanary;)

	size_t capacity;
	size_t size;
	
	char* data;

	ON_STACK_PROTECT(unsigned long long hash;)
	
	Stack* pointer;

	ON_STACK_PROTECT(StkCanary BackCanary;)

};



void StackCtor(Stack* stk, size_t capacity)
{
	assert(stk);
	StackClear(stk)

	stk->size 	  = 0;
	stk->capacity = capacity;
	stk->pointer  = stk;

	stk->data = (char*)calloc(stk->capacity * sizeof(StkElem) + sizeof(StkCanary) * 2, sizeof(char));
	
	assert(stk->data);
	

	*(StkCanary*)(stk->data) = CanaryValue;
	*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;


	for ( size_t i = 0; i < stk->capacity; i++)
		((StkElem*)(stk->data + sizeof(StkCanary)))[i] = POSION_VALUE;


	#ifdef STACK_PROTECT
	ON_STACK_PROTECT(stk->FrontCanary = CanaryValue;)
	ON_STACK_PROTECT(stk->BackCanary  = CanaryValue;)
	#endif

	CheckStack(stk);
}

/*
int StackPush(Stack* stk, StkElem value)
{
	assert(stk);
}
*/