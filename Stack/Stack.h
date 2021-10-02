#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "Print.h"
#include "logger.h"

#define _CONCAT_PROXY(LEFT, RIGHT)		LEFT ## _ ## RIGHT
#define CONCAT(LEFT, RIGHT)				_CONCAT_PROXY(LEFT, RIGHT)
#ifndef LOGFILE
#define LOGFILE 

const char* FILE_LOG = "logfile.txt";

#endif

#define DEBUG

#ifndef PARAMETRS
#define PARAMETRS
	typedef unsigned long long 	StkCanary;
	
	#ifdef USER_DATA_TYPE
		typedef int (*userDataPrint_t)(char* dest, size_t maxlen, StkElem);
		static userDataPrint_t CONCAT(userDataPrint, StkElem) = NULL;  
	#endif
	
	static int 	dump_buff_pos 				= 0;
	static char dump_buffer[BUFSIZ + 10] 	= {}; 
	const StkElem 	POSION_VALUE			= 2699;
	const StkCanary CanaryValue 			= 0xBEAFDEDDEADF00DUL;
#endif




#define _STK_DUMP(format, ...)														\
do{																					\
	int ret = 0;																	\
	if(dump_buff_pos == BUFSIZ - 4)													\
	{																				\
		ret = snprintf(dump_buffer + dump_buff_pos, BUFSIZ - dump_buff_pos, "...");	\
		dump_buff_pos += ret + 1;													\
	} 																				\
	if(dump_buff_pos != BUFSIZ)														\
	{																				\
		ret = snprintf(	dump_buffer + dump_buff_pos, BUFSIZ - dump_buff_pos,		\
						format, ##__VA_ARGS__);										\
		dump_buff_pos += ret;														\
	}																				\
}while(0)

#ifdef USER_DATA_TYPE
	#define _PRINT_VAL(val)	\
		userDataPrint(val);

	#undef USER_DATA_TYPE
#else
	#define _PRINT_VAL(val) 														\
	do{																				\
		int ret = CONCAT(Print, StkElem)( dump_buffer + dump_buff_pos,				\
							BUFSIZ - dump_buff_pos, val);							\
		dump_buff_pos += ret;														\
	}while(0)
#endif



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
	#define StkElem 				int
	#define STACK(TYPE) 			Stack
	#define STK_CTOR(TYPE)			StackCtor
	#define STK_DTOR(TYPE)			StackDtor
	#define STK_POP(TYPE)			StackPop
	#define STK_PUSH(TYPE)			StackPush 
#endif

#define CTOR(TYPE, stk, capacity) 	STK_CTOR(TYPE)												\
									(&stk, capacity,											\
									(StackInfo){#stk, {__FILE__, __PRETTY_FUNCTION__, __LINE__}})

#define DTOR(TYPE, stk)				STK_DTOR(TYPE)	(&stk)
#define POP(TYPE, stk)				STK_POP(TYPE) 	(&stk)
#define PUSH(TYPE, stk, value) 		STK_PUSH(TYPE) 	(&stk, value)


#ifdef ALL_CHECK
	#define CheckStack(TYPE, reason, stk_pointer)											\
				STK_CheckStack(TYPE) 														\
				(reason, stk_pointer, Location{__FILE__, __PRETTY_FUNCTION__, __LINE__})
#else 
	#define CheckStack(TYPE, reason, stk_name, stk_pointer) 
#endif


//// ------------ default ctor ( == 0)
typedef struct
{
	const char* File;
	const char* Function;
	int Line;
} Location;
typedef struct
{
	const char* name;
	
	Location location;
} StackInfo;

typedef struct 
{
	ON_STACK_PROTECT(StkCanary FrontCanary;)

	size_t capacity;
	size_t size;
	
	char* data;
	
	bool initialized;
	unsigned long long hash;
	StackInfo* info;

	ON_STACK_PROTECT(StkCanary BackCanary;)
} STACK(StkElem);

#ifndef STACKClEAR
#define STACKClEAR
	#define StackClear(stk)													\
	do{																		\
		if (stk->initialized) 												\
		{																	\
			pr_warn(LOG_CONSOLE, "Trying to initialize initialized stack!");\
			assert(!"Trying to initialize initialized stack!");				\
			return;															\
		}																	\
	}while(0)
#endif

void STK_CTOR(StkElem) (STACK(StkElem)* stk, const size_t capacity, StackInfo info);
void STK_DTOR(StkElem) (STACK(StkElem)* stk);
void STK_PUSH(StkElem) (STACK(StkElem)* stk, StkElem value);
StkElem STK_POP(StkElem) (STACK(StkElem)* stk);
bool isStackOk(STACK(StkElem)* stk);
void cleanStack(STACK(StkElem)* stk);
void stackDump(	const char* result, const char* reason,
				STACK(StkElem)* stk_pointer, Location loc);


#ifndef FUNCS_INIT
	unsigned long long hash(void* buffer, size_t size);
	unsigned long long rol(unsigned long long value);
#endif


#ifdef RESIZEABLE
	void CONCAT(expandStack,  StkElem)(STACK(StkElem)* stk);
	void CONCAT(squeezeStack, StkElem)(STACK(StkElem)* stk);
#endif




#ifdef ALL_CHECK		
	void STK_CheckStack(StkElem) (	const char* reason, const char* stk_name, STACK(StkElem)* stk_pointer,\
									const char* file, Location loc)
	{
		FILE* fp = fopen(FILE_LOG, "a");

	 	if(!isStackOk(stk_pointer))
		{

			stackDump("Not Ok", reason, stk_pointer, loc);
			
			fprintf(stdout, "%s", dump_buffer);
			fprintf(fp, "%s", dump_buffer);
			
			bzero(dump_buffer, BUFSIZ);
			
			fclose(fp);
			
			assert(!"Stack is not OK, check logfile.txt");
			return;
		}
		
		stackDump("Ok", reason, stk_pointer, loc);
		
		fprintf(fp, "%s", dump_buffer);
		fprintf(stdout, "%s", dump_buffer);
		
		fclose(fp);
		
		bzero(dump_buffer, BUFSIZ);
		
		return;
	}
#undef ALL_CHECK
#endif



void STK_CTOR(StkElem) (STACK(StkElem)* stk, const size_t capacity, StackInfo info)
{

	assert(stk && "bad stk pointer in STK_CTOR");
	StackClear(stk);

	stk->initialized 	= true;
	stk->size 			= 0;
	stk->capacity 		= capacity;

	stk->data 			= (char*)calloc(stk->capacity * sizeof(StkElem) + sizeof(StkCanary) * 2, sizeof(char));

	stk->info 			= (StackInfo*)calloc(1, sizeof(StackInfo));
	
	stk->info->name 			= info.name;
	stk->info->location.File 	= info.location.File;
	stk->info->location.Function = info.location.Function;
	stk->info->location.File 	= info.location.Line;


	assert(stk->data && "Something wrong with stk's data allocation");
	assert(stk->info && "Something wrong with stk's info allocation");


	*(StkCanary*)(stk->data) = CanaryValue;
	*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;
	
	stk->data += sizeof(StkCanary);

	for ( size_t i = 0; i < stk->capacity; i++)
		((StkElem*)(stk->data))[i] = POSION_VALUE;

	stk->hash = hash(stk->data, stk->capacity * sizeof(StkElem));

	ON_STACK_PROTECT(stk->FrontCanary = CanaryValue;)
	ON_STACK_PROTECT(stk->BackCanary  = CanaryValue;)

	CheckStack(StkElem ,"Stack is beind checked after Constructor", stk);
}

void STK_DTOR(StkElem) (STACK(StkElem)* stk)
{
	assert(stk && "bad stk pointer in STK_DTOR");

	cleanStack(stk);
	
	stk->data -= sizeof(StkCanary);

	free(stk->data);
	free(stk->info);

}


StkElem STK_POP(StkElem) (STACK(StkElem)* stk)
{
	assert(stk && "bad stk pointer in STK_POP");


	CheckStack(StkElem, "Stack is being checked before POPing", stk);


	if (stk->size == 0)
	{
		printf("Stack is empty!\n");
		return POSION_VALUE;
	}

	StkElem value = ((StkElem*)(stk->data))[--stk->size];
	((StkElem*)(stk->data))[stk->size] = POSION_VALUE;


	if (stk->size * 3 <= stk->capacity)
		CONCAT(squeezeStack, StkElem)(stk);


	stk->hash = hash(stk->data, stk->capacity * sizeof(StkElem));



	CheckStack(StkElem, "Stack is being checked after POPing", stk->info->name, stk);

	
	return value;
}

void STK_PUSH(StkElem) (STACK(StkElem)* stk, const StkElem value)
{
	assert(stk && "bad stk pointer in STK_PUSH");

	CheckStack(StkElem, "Stack is being checked before PUSHing", stk->info->name, stk);

	printf("size is %zu and capacity is %zu\n", stk->size, stk->capacity);
	
	if(stk->size < stk->capacity)
		((StkElem*)(stk->data))[stk->size++] = value;
	else 
	{
		printf("Stack %s is full\n", stk->info->name);
		CONCAT(expandStack, StkElem)(stk);
		((StkElem*)(stk->data))[stk->size++] = value;
	}

	stk->hash = hash(stk->data, stk->capacity * sizeof(StkElem));

	CheckStack(StkElem, "Stack is being checked after PUSHing", stk->info->name, stk);
}


bool isStackOk(STACK(StkElem)* stk)
{
	if (hash(stk->data, stk->capacity * sizeof(StkElem)) != stk->hash)
		return false;

	stk->data -= sizeof(StkCanary); 
	
	if (*(StkCanary*)(stk->data) != CanaryValue || (*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) != CanaryValue))
		return false;

	stk->data += sizeof(StkCanary); 
	
	ON_STACK_PROTECT(if(stk->FrontCanary != stk->BackCanary || stk->FrontCanary != CanaryValue) return false;)
	
	return true;
}

#ifndef FUNCS_INIT
#define FUNCS_INIT

unsigned long long hash(void* buffer, const size_t size)
{
	unsigned char* buf = (unsigned char*)buffer;
	unsigned long long hash = rol(*buf);
	
	for( size_t i = 0; i < size; i ++)
		hash = rol(hash) ^ buf[i];

	return hash;
}

unsigned long long rol(unsigned long long value)
{
	return (value << 1) | (value >> (8 * sizeof(value) - 1));
}

#endif


void stackDump(	const char* result, const char* reason, \
				STACK(StkElem)* stk, Location loc) 
{						
	printf("\n%s\n", __PRETTY_FUNCTION__);

	// fprintf(stream, "Stack: %s (%s) [%p]\n"
					// "called from : %s;\n"
					// "reason: %s;\n"
					// "file: %s\n{\n", stk_name, result, stk_pointer, func, reason, file);
	_STK_DUMP
	(
		"Stack: %s (%s) [%p]\n"
		"Created:\n"
		"\tfunction: %s\n"
		"\tfile: %s\n"
		"\tline: %d\n"
		"Dump called from:"
		"\tfunction: %s;\n"
		"\tfile: %s\n"
		"\tline: %d\n"
		"reason: %s;\n{",
		stk->info->name, result, stk, 
		stk->info->creationFunction, stk->info->creationFile, stk->info->creationLine,
		
		func, reason
	);

	// fprintf(stream, "\tsize = %zu\n"
					// "\tcapacity = %zu\n"
					// "\tdata [%p]\n" 
					// "\t{\n", stk_pointer->size, stk_pointer->capacity, stk_pointer->data);
	
	_STK_DUMP
	(
			"\tsize = %zu\n"
			"\tcapacity = %zu\n"
			"\tdata [%p]\n" 
			"\t{\n", stk->size, stk->capacity, stk->data
	);


	#ifdef DEBUG
		_STK_DUMP
		(
			"\t\tFrontCanary: %llu\n",
			*((StkCanary*)(stk->data-sizeof(StkCanary))) 
		);
	#endif

	for ( size_t i = 0; i < stk->capacity; i++)
		if( ((StkElem*)( stk->data ))[i] == POSION_VALUE)
			// fprintf(stream, "\t\t[%2zu] = NAN (POISON!)\n", i);
		_STK_DUMP
		(
			"\t\t[%2zu] = NAN (POISON!)\n", i
		);
		else
		{	
			
			// fprintf(stream, "\t\t*[%2zu] = ", i); 
			_STK_DUMP
			(
				"\t\t*[%2zu] = ", i
			);
			_PRINT_VAL
			(
				((StkElem*)(stk->data))[i]
			);
		}

	#ifdef DEBUG
		// fprintf(stream, "\t\tBackCanary:  %llu\n", *((StkCanary*)(stk_pointer->data+sizeof(StkElem)*stk_pointer->capacity)) );
		_STK_DUMP
		(
			"\t\tBackCanary:  %llu\n", 
			*((StkCanary*)(stk->data+sizeof(StkElem)*stk->capacity))
		);
	#endif

	// fprintf(stream, "\n\t}\n\thash: %llu\n}\n============++++===============\n\n\n", stk_pointer->hash);
	_STK_DUMP
	(
		"\n\t}\n\thash: %llu\n}\n============++++===============\n\n\n",
		stk->hash
	);

	dump_buff_pos = 0;
}



void cleanStack(STACK(StkElem)* stk)
{
	assert(stk && "bad stk pointer in cleanStack");

	for(size_t i = 0; i < stk->capacity; i ++)
		((StkElem*) (stk->data))[i] = POSION_VALUE;

	stk->hash = hash(stk->data, stk->capacity * sizeof(StkElem));
}

#ifdef RESIZEABLE
	void CONCAT(expandStack, StkElem)(STACK(StkElem)* stk)
	{
		
		printf("================EXPANDING===============\n");
		assert(stk && "bad stk pointer in expandStack");

		stk->data -= sizeof(StkCanary);
		stk->data = (char*)realloc(stk->data, stk->capacity * sizeof(StkElem) * 2 + sizeof(StkCanary) * 2);

		assert(stk->data && "Bad reallocation while expanding");

		
		stk->capacity = stk->capacity * 2;

		*(StkCanary*)(stk->data) = CanaryValue;
		*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;

		stk->data += sizeof(StkCanary);

		for( size_t i = stk->size; i < stk->capacity; i++)
			((StkElem*)(stk->data))[i] = POSION_VALUE;

		stk->hash = hash(stk->data, stk->capacity * sizeof(StkElem));
	}

	void CONCAT(squeezeStack, StkElem)(STACK(StkElem)* stk)
	{
		printf("================SQUEEZING===============\n");
		assert(stk && "bad stk pointer in squeezeStack");

		stk->data -= sizeof(StkCanary);
		stk->data = (char*)realloc(stk->data, stk->capacity * sizeof(StkElem) / 2 + sizeof(StkCanary) * 2);

		assert(stk->data && "Bad reallocation while expanding");

		stk->capacity = stk->capacity / 2;

		*(StkCanary*)(stk->data) = CanaryValue;
		*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;

		stk->data += sizeof(StkCanary);

		stk->hash = hash(stk->data, stk->capacity * sizeof(StkElem));
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