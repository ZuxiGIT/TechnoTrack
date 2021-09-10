#include <assert.h>
#include <stdlib.h>
#include "Print.hpp"


#ifndef LOGFILE
#define LOGFILE 

const char* FILE_LOG = "logfile.txt";

#endif

#define DEBUG



#ifdef DEBUG
	#define ALL_CHECK
	#define PROTECT
	#define RESIZEABLE
#endif

#ifdef PROTECT
	#define ON_STACK_PROTECT(code)	code
#else
	#define ON_STACK_PROTECT(code)
#endif

#define	NAME_OF_ARG(ARG)		#ARG
#define STACK_CONCAT(TYPE) 		Stack 		## _ ## TYPE
#define CTOR_CONCAT(TYPE)		StackCtor 	## _ ## TYPE	
#define DTOR_CONCAT(TYPE)		StackDtor	## _ ## TYPE 
#define POP_CONCAT(TYPE)		StackPop 	## _ ## TYPE
#define PUSH_CONCAT(TYPE) 		StackPush	## _ ## TYPE
#define CheckStack_CONCAT(TYPE)	CheckStack  ## _ ## TYPE

#ifdef StkElem
	#define STACK(TYPE) 			STACK_CONCAT(TYPE)
	#define STK_CTOR(TYPE)			CTOR_CONCAT(TYPE)
	#define STK_DTOR(TYPE)			DTOR_CONCAT(TYPE)
	#define STK_POP(TYPE)			POP_CONCAT(TYPE)
	#define STK_PUSH(TYPE)			PUSH_CONCAT(TYPE)
	#define STK_CheckStack(TYPE) 	CheckStack_CONCAT(TYPE)
#else
	#define STACK(TYPE) 			Stack
	#define STK_CTOR(TYPE)			StackCtor
	#define STK_DTOR(TYPE)			StackDtor
	#define STK_POP(TYPE)			StackPop
	#define STK_PUSH(TYPE)			StackPush 
#endif

#define CTOR(TYPE, stk, capacity) 	STK_CTOR(TYPE) 		(#stk, &stk, capacity)
#define DTOR(TYPE, stk)				STK_DTOR(TYPE)		(#stk, &stk)
#define POP(TYPE, stk)				STK_POP(TYPE) 		(#stk, &stk)
#define PUSH(TYPE, stk, value) 		STK_PUSH(TYPE) 		(#stk, &stk, value)


#ifdef ALL_CHECK
	#define CheckStack(TYPE, reason, stk_name, stk_pointer)	STK_CheckStack(TYPE)	(reason, stk_name, stk_pointer, __FILE__, __PRETTY_FUNCTION__)
#else 
	#define heckStack(TYPE, reason, stk_name, stk_pointer) 
#endif

#ifndef PARAMETRS
#define PARAMETRS
	typedef unsigned long long 	StkCanary;

	const StkElem 	POSION_VALUE	= 2699;
	const StkCanary CanaryValue 	= 0xBEAFDEDDEADF00D;
#endif


struct STACK(StkElem)
{
	ON_STACK_PROTECT(StkCanary FrontCanary = CanaryValue;)

	size_t capacity = 0;
	size_t size = 0;
	
	char* data = nullptr;
	
	bool initialized = false;
	unsigned long long hash = 0;

	ON_STACK_PROTECT(StkCanary BackCanary = CanaryValue;)
};

#ifndef STACKClEAR
#define STACKClEAR
	#define StackClear(stk) if (stk->initialized) { assert(!"Trying to initialize initialized stack!");}
#endif

void STK_CTOR(TYPE) (STACK(StkElem)* stk, size_t capacity = 10);
void STK_DTOR(TYPE) (const char* stk_name, STACK(StkElem)* stk);
void STK_PUSH(StkElem) (const char* stk_name, STACK(StkElem)* stk, StkElem value);
StkElem STK_POP(StkElem) (const char* stk_name, STACK(StkElem)* stk);
bool isStackOk(STACK(StkElem)* stk);
void CleanStack(STACK(StkElem)* stk);
void StackDump(	FILE* stream, const char* result, const char* reason, const char* stk_name, \
				STACK(StkElem)* stk_pointer, const char* file, const char* func);


#ifndef FUNCS_INIT
	unsigned long long Hash(void* buffer, size_t size);
	unsigned long long Rol(unsigned long long value);
#endif


#ifdef RESIZEABLE
	void ExpandStack(STACK(StkElem)* stk);
	void SqueezeStack(STACK(StkElem)* stk);
#endif




#ifdef ALL_CHECK		
	void STK_CheckStack(StkElem) (	const char* reason, const char* stk_name, STACK(StkElem)* stk_pointer,\
									const char* file, const char* func)
	{
		FILE* fp = fopen(FILE_LOG, "a");
		const char* resultOK 	= "OK";
		const char* resultNotOK = "Not OK";
	 	if(!isStackOk(stk_pointer))
		{
			StackDump(stdout, resultNotOK, reason, stk_name, stk_pointer, file, func);
			StackDump(fp, 	  resultNotOK, reason, stk_name, stk_pointer, file, func);
			fclose(fp);
			assert(!"Stack is not OK, check logfile.txt");
		}
		StackDump(fp, resultOK, reason, stk_name, stk_pointer, file, func);
		fclose(fp);
	}
	#undef ALL_CHECK
#endif



void STK_CTOR(StkElem) (const char* stk_name, STACK(StkElem)* stk, const size_t capacity)
{
	assert(stk && "bad stk pointer in STK_CTOR");
	StackClear(stk)
	
	stk->initialized 	= true;
	stk->size 			= 0;
	stk->capacity 		= capacity;

	stk->data 			= (char*)calloc(stk->capacity * sizeof(StkElem) + sizeof(StkCanary) * 2, sizeof(char));
	
	assert(stk->data && "Something wrong with stk's data allocation");
	

	*(StkCanary*)(stk->data) = CanaryValue;
	*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;
	
	stk->data += sizeof(StkCanary);

	for ( size_t i = 0; i < stk->capacity; i++)
		((StkElem*)(stk->data))[i] = POSION_VALUE;

	stk->hash = Hash(stk->data, stk->capacity * sizeof(StkElem));

	ON_STACK_PROTECT(stk->FrontCanary = CanaryValue;)
	ON_STACK_PROTECT(stk->BackCanary  = CanaryValue;)

	CheckStack(StkElem ,"Stack is beind checked after Constructor", stk_name, stk);
}

void STK_DTOR(StkElem) (const char* stk_name, STACK(StkElem)* stk)
{
	assert(stk && "bad stk pointer in STK_DTOR");

	CleanStack(stk);
	
	stk->data -= sizeof(StkCanary);

	free(stk->data);

}


StkElem STK_POP(StkElem) (const char* stk_name, STACK(StkElem)* stk)
{
	assert(stk && "bad stk pointer in STK_POP");

	CheckStack(StkElem, "Stack is being checked before POPing", stk_name, stk);

	if (stk->size == 0)
	{
		printf("Stack is empty!\n");
		return POSION_VALUE;
	}

	StkElem value = ((StkElem*)(stk->data))[--stk->size];
	((StkElem*)(stk->data))[stk->size] = POSION_VALUE;

	if (stk->size * 3 <= stk->capacity)
		SqueezeStack(stk);

	stk->hash = Hash(stk->data, stk->capacity * sizeof(StkElem));


	CheckStack(StkElem, "Stack is being checked after POPing", stk_name, stk);
	return value;
}

void STK_PUSH(StkElem) (const char* stk_name, STACK(StkElem)* stk, const StkElem value)
{
	assert(stk && "bad stk pointer in STK_PUSH");

	CheckStack(StkElem, "Stack is being checked before PUSHing", stk_name, stk);

	printf("size is %zu and capacity is %zu\n", stk->size, stk->capacity);
	
	if(stk->size < stk->capacity)
		((StkElem*)(stk->data))[stk->size++] = value;
	else 
	{
		printf("Stack %s is full\n", stk_name);
		ExpandStack(stk);
		((StkElem*)(stk->data))[stk->size++] = value;
	}

	stk->hash = Hash(stk->data, stk->capacity * sizeof(StkElem));

	CheckStack(StkElem, "Stack is being checked after PUSHing", stk_name, stk);
}


bool isStackOk(STACK(StkElem)* stk)
{
	if (Hash(stk->data, stk->capacity * sizeof(StkElem)) != stk->hash)
		return false;

	stk->data -= sizeof(StkCanary); 
	
	if (*(StkCanary*)(stk->data) != CanaryValue || (*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) != CanaryValue))
		return false;

	stk ->data += sizeof(StkCanary); 
	
	ON_STACK_PROTECT(if(stk->FrontCanary != stk->BackCanary || stk->FrontCanary != CanaryValue) return false;)
	
	return true;
}

#ifndef FUNCS_INIT
#define FUNCS_INIT

unsigned long long Hash(void* buffer, const size_t size)
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

#endif


void StackDump(	FILE* stream, const char* result, const char* reason, const char* stk_name, \
				STACK(StkElem)* stk_pointer, const char* file, const char* func) 
{																																																
	fprintf(stream, "Stack: %s (%s) [%p]\n"
					"called from : %s;\n"
					"reason: %s;\n"
					"file: %s\n{\n", stk_name, result, stk_pointer, func, reason, file);

	fprintf(stream, "\tsize = %zu\n"
					"\tcapacity = %zu\n"
					"\tdata [%p]\n" 
					"\t{\n", stk_pointer->size, stk_pointer->capacity, stk_pointer->data);
	
	#ifdef DEBUG
		fprintf(stream, "\t\tFrontCanary: %llu\n", *((StkCanary*)(stk_pointer->data-sizeof(StkCanary))) );
	#endif

	for ( size_t i = 0; i < stk_pointer->capacity; i++)
		if( ((StkElem*)( stk_pointer->data ))[i] == POSION_VALUE)
			fprintf(stream, "\t\t[%2zu] = NAN (POISON!)\n", i);
		else
		{	
			fprintf(stream, "\t\t*[%2zu] = ", i); 
			Print(stream, ((StkElem*)(stk_pointer->data))[i]);
		}

	#ifdef DEBUG
		fprintf(stream, "\t\tBackCanary:  %llu\n", *((StkCanary*)(stk_pointer->data+sizeof(StkElem)*stk_pointer->capacity)) );
	#endif

	fprintf(stream, "\n\t}\n\tHash: %llu\n}\n============++++===============\n\n\n", stk_pointer->hash);
}



void CleanStack(STACK(StkElem)* stk)
{
	assert(stk && "bad stk pointer in CleanStack");

	for(size_t i = 0; i < stk->capacity; i ++)
		((StkElem*) (stk->data))[i] = POSION_VALUE;

	stk->hash = Hash(stk->data, stk->capacity * sizeof(StkElem));
}

#ifdef RESIZEABLE
	void ExpandStack(STACK(StkElem)* stk)
	{
		
		printf("================EXPANDING===============\n");
		assert(stk && "bad stk pointer in ExpandStack");

		stk->data -= sizeof(StkCanary);
		stk->data = (char*)realloc(stk->data, stk->capacity * sizeof(StkElem) * 2 + sizeof(StkCanary) * 2);

		assert(stk->data && "Bad reallocation while expanding");

		
		stk->capacity = stk->capacity * 2;

		*(StkCanary*)(stk->data) = CanaryValue;
		*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;

		stk->data += sizeof(StkCanary);

		for( size_t i = stk->size; i < stk->capacity; i++)
			((StkElem*)(stk->data))[i] = POSION_VALUE;

		stk->hash = Hash(stk->data, stk->capacity * sizeof(StkElem));
	}

	void SqueezeStack(STACK(StkElem)* stk)
	{
		printf("================SQUEEZING===============\n");
		assert(stk && "bad stk pointer in SqueezeStack");

		stk->data -= sizeof(StkCanary);
		stk->data = (char*)realloc(stk->data, stk->capacity * sizeof(StkElem) / 2 + sizeof(StkCanary) * 2);

		assert(stk->data && "Bad reallocation while expanding");

		stk->capacity = stk->capacity / 2;

		*(StkCanary*)(stk->data) = CanaryValue;
		*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;

		stk->data += sizeof(StkCanary);

		stk->hash = Hash(stk->data, stk->capacity * sizeof(StkElem));
	}

#undef RESIZEABLE
#endif



#ifdef DEBUG
	#undef ALL_CHECK
	#undef PROTECT
	#undef DEBUG
#endif

#ifdef PROTECT
	#undef PROTECT
#endif

#undef StkElem