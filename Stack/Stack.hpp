#include <assert.h>
#include <stdlib.h>
#include "Print.hpp"


#define DEBUG
#define PROTECT




#ifdef DEBUG
#define ALL_CHECK
#endif

#ifdef PROTECT
#define ON_STACK_PROTECT(code) code
#else
#define ON_STACK_PROTECT(code)
#endif


typedef unsigned long long StkCanary;
typedef int 			   StkElem;

const StkElem 	POSION_VALUE	= 2699;
const StkCanary CanaryValue 	= 0xBEAFDEDDEADF00D;



struct Stack
{

	ON_STACK_PROTECT(StkCanary FrontCanary;)

	size_t capacity;
	size_t size;
	
	char* data;

	ON_STACK_PROTECT(unsigned long long hash;)
	
	Stack* pointer;
	unsigned long long hash;

	ON_STACK_PROTECT(StkCanary BackCanary;)

};




void StackCtor(Stack* stk, size_t capacity = 10);
bool isStackOk(Stack* stk);
unsigned long long Hash(void* buffer, size_t size);
unsigned long long Rol(unsigned long long value);


#define CheckStack(reason, stk)	{ if(!isStackOk(stk)) { const char* res = "Not Ok"; FStackDump(stk, reason) /*assert(!"Stack is not OK, check logfile");*/}}

#define FStackDump(stk, reason) 	{ 	FILE* fp = fopen("logfile.txt", "a");																												\
									if(!fp)	assert(!"File was not opened!");																											\
									fprintf(fp, "Stack(%s) [%p]\ncalled from : %s;\nreason: %s;\nfile: %s\n{\n", res, stk, __PRETTY_FUNCTION__, reason, __FILE__);						\
									fprintf(fp, "\tsize = %zu\n\tcapacity = %zu\n\tdata [%p]\n\t{\n", stk->size, stk->capacity, stk->data);												\
									for ( size_t i = 0; i < stk->capacity; i++)																											\
									{																																					\
										if( ( (StkElem*)(stk->data + sizeof(StkCanary)) )[i] == POSION_VALUE)																			\
											fprintf(fp, "\t\t[%zu] = NAN (POISON!)\n", i);																								\
										else																																			\
											{fprintf(fp, "\t\t*[%zu] = ", i); Print(fp, ( (StkElem*)(stk->data + sizeof(StkCanary)) )[i]);}												\
									}																																					\
									fprintf(fp, "\n\t}\n}");																															\
									fclose(fp);																																			}



#define StackClear(stk) if (stk == stk->pointer) { assert(!"Trying to initialize initialized stack!");}







void StackCtor(Stack* stk, size_t capacity)
{
	assert(stk);
	StackClear(stk)
	
	stk->size 	  	= 0;
	stk->capacity 	= capacity;
	stk->pointer  	= stk;

	stk->data 		= (char*)calloc(stk->capacity * sizeof(StkElem) + sizeof(StkCanary) * 2, sizeof(char));
	
	assert(stk->data);
	

	*(StkCanary*)(stk->data) = CanaryValue;
	*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;
	stk->data += sizeof(StkCanary);

	for ( size_t i = 0; i < stk->capacity; i++)
		((StkElem*)(stk->data))[i] = POSION_VALUE;

	stk->hash = Hash(stk->data, stk->capacity);



	ON_STACK_PROTECT(stk->FrontCanary = CanaryValue;)
	ON_STACK_PROTECT(stk->BackCanary  = CanaryValue;)

	#ifdef ALL_CHECK
	CheckStack("Stack is checking after Ctor", stk);
	#endif
}


bool isStackOk(Stack* stk)
{
	if (Hash(stk->data, stk->capacity) != stk->hash)
		return false;
	if (stk != stk->pointer)
		return false;

	stk->data -= sizeof(StkCanary); 
	if (*(StkCanary*)(stk->data) != CanaryValue || (*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) != CanaryValue))
		return false;
	stk->data += sizeof(StkCanary); 
	
	ON_STACK_PROTECT(if(stk->FrontCanary != stk->BackCanary || stk->FrontCanary != CanaryValue) return false;)
	
	return true;
}


unsigned long long Hash(void* buffer, size_t size)
{
	unsigned char* buf = (unsigned char*)buffer;
	unsigned long long hash = Rol(*buf);
	for( size_t i = 0; i < size; i ++)
		hash = Rol(hash) ^ buf[i];

	return hash;
}

unsigned long long Rol(unsigned long long value)
{
	return (value << 1) | (value >> (8 * sizeof(value) - 1));
}