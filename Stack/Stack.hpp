#include <assert.h>
#include <stdlib.h>
#include "Print.hpp"

#define DEBUG
#define PROTECT


#define	NAME_OF_ARG(ARG)	#ARG
#define STACK_CONCAT(TYPE) 	Stack 		## _ ## TYPE
#define CTOR_CONCAT(TYPE)	StackCtor 	## _ ## TYPE	
#define POP_CONCAT(TYPE)	StackPop 	## _ ## TYPE
#define PUSH_CONCAT(TYPE) 	StackPush	## _ ## TYPE

#ifdef DEBUG
	#define ALL_CHECK
#endif

#ifdef PROTECT
	#define ON_STACK_PROTECT(code)	code
#else
	#define ON_STACK_PROTECT(code)
#endif


#ifdef ALL_CHECK
	#define CheckStack(reason, stk_name, stk_pointer)				 														\
	{																														\
	 	if(isStackOk(stk_pointer))																							\
		{ 																													\
			const char* file = __FILE__;																					\
			const char* func = __PRETTY_FUNCTION__;																			\
			const char* result = "Not OK";																					\
		 	FStackDump(result, reason, stk_name, (stk_pointer), file, func) /*assert(!"Stack is not OK, check logfile");*/ 	\
		}																													\
	} 
#else
	#define CheckStack(reason, stk_name, stk_pointer)
#endif


#ifdef StkElem
	#define STACK(TYPE) 	STACK_CONCAT(TYPE)
	#define STK_CTOR(TYPE)	CTOR_CONCAT(TYPE)
	#define STK_POP(TYPE)	POP_CONCAT(TYPE)
	#define STK_PUSH(TYPE)	PUSH_CONCAT(TYPE)
#else
	#define STACK(TYPE) 	Stack
	#define STK_CTOR(TYPE)	StackCtor
	#define STK_POP(TYPE)	StackPop
	#define STK_PUSH(TYPE)	StackPush
#endif


typedef unsigned long long 	StkCanary;
//typedef int 				StkElem;


const StkElem 	POSION_VALUE	= 2699;
const StkCanary CanaryValue 	= 0xBEAFDEDDEADF00D;



struct STACK(StkElem)
{

	ON_STACK_PROTECT(StkCanary FrontCanary;)

	size_t capacity;
	size_t size;
	
	char* data;
	
	STACK(StkElem)* pointer;
	unsigned long long hash;

	ON_STACK_PROTECT(StkCanary BackCanary;)

};



void StackCtor(STACK(StkElem)* stk, size_t capacity = 10);
bool isStackOk(STACK(StkElem)* stk);
unsigned long long Hash(void* buffer, size_t size);
unsigned long long Rol(unsigned long long value);


#define CTOR(TYPE, stk, capacity) 	STK_CTOR(TYPE) 	(#stk, &stk, capacity)
#define POP(TYPE, stk)				STK_POP(TYPE) 	(#stk, &stk)
#define PUSH(TYPE, stk, value) 		STK_PUSH(TYPE) 	(#stk, &stk, value)


#define FStackDump(result, reason, stk_name, stk_pointer, file, func)																							\
	{ 																																							\
		FILE* fp = fopen("logfile.txt", "a");																													\
		if(!fp)	assert(!"File was not opened!");																												\
																																								\
		fprintf(fp, "Stack: %s (%s) [%p]\ncalled from : %s;\nreason: %s;\nfile: %s\n{\n", stk_name, result, stk_pointer, __PRETTY_FUNCTION__, reason, file);	\
		fprintf(fp, "\tsize = %zu\n\tcapacity = %zu\n\tdata [%p]\n\t{\n", stk_pointer->size, stk_pointer->capacity, stk_pointer->data);							\
																																								\
		for ( size_t i = 0; i < stk_pointer->capacity; i++)																										\
			if( ( (StkElem*)(stk_pointer->data) )[i] == POSION_VALUE)																							\
				fprintf(fp, "\t\t[%2zu] = NAN (POISON!)\n", i);																									\
			else																																				\
				{fprintf(fp, "\t\t*[%2zu] = ", i); Print(fp, ( (StkElem*)(stk_pointer->data) )[i]);}															\
																																								\
		fprintf(fp, "\n\t}\n}\n============++++===============\n\n\n");																							\
		fclose(fp);																																				\
	}


#define StackClear(stk) if (stk == stk->pointer) { assert(!"Trying to initialize initialized stack!");}


void STK_CTOR(StkElem) (const char* stk_name, STACK(StkElem)* stk, size_t capacity)
{
	assert(stk);
	StackClear(stk)
	
	stk->size 	= 0;
	stk->capacity 	= capacity;
	stk->pointer  	= stk;

	stk->data 	= (char*)calloc(stk->capacity * sizeof(StkElem) + sizeof(StkCanary) * 2, sizeof(char));
	
	assert(stk->data);
	

	*(StkCanary*)(stk->data) = CanaryValue;
	*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;
	stk->data += sizeof(StkCanary);

	for ( size_t i = 0; i < stk->capacity; i++)
		((StkElem*)(stk->data))[i] = POSION_VALUE;

	stk->hash = Hash(stk->data, stk->capacity);



	ON_STACK_PROTECT(stk->FrontCanary = CanaryValue;)
	ON_STACK_PROTECT(stk->BackCanary  = CanaryValue;)

	CheckStack("Stack is beind checked after Constructor", stk_name, stk)
}


bool isStackOk(STACK(StkElem)* stk)
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

StkElem STK_POP(StkElem) (const char* stk_name, STACK(StkElem)* stk)
{
	assert(stk);

	CheckStack("Stack is being checked before POPing", stk_name, stk)

	if (stk->size == 0)
	{
		printf("Stack is empty!\n");
		abort();
	}

	StkElem value = ((StkElem*)(stk->data))[--stk->size];
	((StkElem*)(stk->data))[stk->size] = POSION_VALUE;

	stk->hash = Hash(stk->data, stk->capacity);


	CheckStack("Stack is being checked after POPing", stk_name, stk)
	printf("END OF POP\n");
	return value;
}

void STK_PUSH(StkElem) (const char* stk_name, STACK(StkElem)* stk, StkElem value)
{
	assert(stk);

	CheckStack("Stack is being checked before PUSHing", stk_name, stk)

	printf("size is %zu and capacity is %zu\n", stk->size, stk->capacity);
	if(stk->size < stk->capacity)
		((StkElem*)(stk->data))[stk->size++] = value;
	else printf("Stack %s is full\n", stk_name);

	stk->hash = Hash(stk->data, stk->capacity);

	CheckStack("Stack is being checked after PUSHing", stk_name, stk)
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