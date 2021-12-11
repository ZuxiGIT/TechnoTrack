#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "Print.h"
#include "../logger/logger.h"

//TODO
// User data printing
// dumping through logger
// determine defines ( lots of useless defines)

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
	const StkElem 	STK_POISON_VALUE			= 2699;
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
#define STACK_CONCAT(TYPE) 			Stack 		## _ ## TYPE
#define CTOR_CONCAT(TYPE)			StackCtor 	## _ ## TYPE	
#define DTOR_CONCAT(TYPE)			StackDtor	## _ ## TYPE 
#define POP_CONCAT(TYPE)			StackPop 	## _ ## TYPE
#define PUSH_CONCAT(TYPE) 			StackPush	## _ ## TYPE
#define checkStack_CONCAT(TYPE)		checkStack  ## _ ## TYPE
#define isStackOK_CONCAT(TYPE)		isStackOK	## _ ## TYPE
#define cleanStack_CONCAT(TYPE)		cleanStack	## _ ## TYPE
#define stackDump_CONCAT(TYPE)		stackDump 	## _ ## TYPE
#define expandStack_CONCAT(TYPE)	expandStack ## _ ## TYPE
#define squeezeStack_CONCAT(TYPE) 	squeezeStack ## _ ## TYPE

#ifdef StkElem
	#define STACK(TYPE) 			STACK_CONCAT(TYPE)
	#define STK_CTOR(TYPE)			CTOR_CONCAT(TYPE)
	#define STK_DTOR(TYPE)			DTOR_CONCAT(TYPE)
	#define STK_POP(TYPE)			POP_CONCAT(TYPE)
	#define STK_PUSH(TYPE)			PUSH_CONCAT(TYPE)
	#define STK_checkStack(TYPE)	checkStack_CONCAT(TYPE)
	#define STK_isStackOk(TYPE)		isStackOK_CONCAT(TYPE)
	#define STK_cleanStack(TYPE)	cleanStack_CONCAT(TYPE)
	#define STK_stackDump(TYPE)		stackDump_CONCAT(TYPE)
	#define STK_expandStack(TYPE) 	expandStack_CONCAT(TYPE)
	#define STK_squeezeStack(TYPE)	squeezeStack_CONCAT(TYPE)
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
	#define checkStack(TYPE, reason, stk_pointer)											\
				STK_checkStack(TYPE) 														\
				(reason, stk_pointer, (Location){__FILE__, __PRETTY_FUNCTION__, __LINE__})
#else 
	#define checkStack(TYPE, reason, stk_name, stk_pointer) 
#endif


#ifndef STRUCTS_INIT
	#define STRUCTS_INIT
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
#endif

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
	#define StackClear(stk)														\
	do{																			\
		if (stk->initialized) 													\
		{																		\
			pr_warn(LOG_CONSOLE, "Trying to initialize initialized stack!\n");	\
			assert(!"Trying to initialize initialized stack");					\
			return;																\
		}																		\
	}while(0)
#endif

void STK_CTOR(StkElem) (STACK(StkElem)* stk, const size_t capacity, StackInfo info);
void STK_DTOR(StkElem) (STACK(StkElem)* stk);
void STK_PUSH(StkElem) (STACK(StkElem)* stk, StkElem value);

StkElem STK_POP(StkElem) (STACK(StkElem)* stk);

bool STK_isStackOk(StkElem)(STACK(StkElem)* stk);

void STK_cleanStack(StkElem)(STACK(StkElem)* stk);
void STK_stackDump(StkElem)(const char* result, const char* reason,
							STACK(StkElem)* stk_pointer, Location loc);


#ifndef FUNCS_INIT
	unsigned long long hash(void* buffer, size_t size);
	unsigned long long rol(unsigned long long value);
#endif


#ifdef RESIZEABLE
	void STK_expandStack(StkElem)(STACK(StkElem)* stk);
	void STK_squeezeStack(StkElem)(STACK(StkElem)* stk);
#endif




#ifdef ALL_CHECK		
	void STK_checkStack(StkElem) (	const char* reason,STACK(StkElem)* stk_pointer, Location loc)
	{
		FILE* fp = fopen(FILE_LOG, "a");

	 	if(!STK_isStackOk(StkElem)(stk_pointer))
		{

			STK_stackDump(StkElem)("Not Ok", reason, stk_pointer, loc);
			
			// fprintf(stdout, "%s", dump_buffer);
			fprintf(fp, "%s", dump_buffer);
			
			bzero(dump_buffer, BUFSIZ);
			
			fclose(fp);
			
			assert(!"Stack is not OK, check logfile.txt");
			return;
		}
		
		STK_stackDump(StkElem)("Ok", reason, stk_pointer, loc);
		
		fprintf(fp, "%s", dump_buffer);
		// fprintf(stdout, "%s", dump_buffer);
		
		bzero(dump_buffer, BUFSIZ);
		
		fclose(fp);
		
		
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
	
	stk->info->name 				= info.name;
	stk->info->location.File 		= info.location.File;
	stk->info->location.Function 	= info.location.Function;
	stk->info->location.Line 		= info.location.Line;


	assert(stk->data && "Something wrong with stk's data allocation");
	assert(stk->info && "Something wrong with stk's info allocation");


	*(StkCanary*)(stk->data) = CanaryValue;
	*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;
	
	stk->data += sizeof(StkCanary);

	for ( size_t i = 0; i < stk->capacity; i++)
		((StkElem*)(stk->data))[i] = STK_POISON_VALUE;

	stk->hash = hash(stk->data - sizeof(StkCanary),
					 stk->capacity * sizeof(StkElem) + 2 * sizeof(StkCanary));

	ON_STACK_PROTECT(stk->FrontCanary = CanaryValue;)
	ON_STACK_PROTECT(stk->BackCanary  = CanaryValue;)

	checkStack(StkElem ,"Stack is beind checked after Constructor", stk);
}

void STK_DTOR(StkElem) (STACK(StkElem)* stk)
{
	assert(stk && "bad stk pointer in STK_DTOR");

	STK_cleanStack(StkElem)(stk);
	
	stk->data -= sizeof(StkCanary);

	free(stk->data);
	free(stk->info);

}


StkElem STK_POP(StkElem) (STACK(StkElem)* stk)
{
	assert(stk && "bad stk pointer in STK_POP");


	checkStack(StkElem, "Stack is being checked before POPing", stk);


	if (stk->size == 0)
	{
		pr_warn(LOG_CONSOLE, "Stack is empty!\n");
		// printf("Stack is empty!\n");
		return STK_POISON_VALUE;
	}

	StkElem value = ((StkElem*)(stk->data))[--stk->size];
	((StkElem*)(stk->data))[stk->size] = STK_POISON_VALUE;


	if (stk->size * 3 <= stk->capacity)
		STK_squeezeStack(StkElem)(stk);


	stk->hash = hash(stk->data - sizeof(StkCanary),
					 stk->capacity * sizeof(StkElem) + 2 * sizeof(StkCanary));



	checkStack(StkElem, "Stack is being checked after POPing", stk);

	
	return value;
}

void STK_PUSH(StkElem) (STACK(StkElem)* stk, const StkElem value)
{
	assert(stk && "bad stk pointer in STK_PUSH");

	checkStack(StkElem, "Stack is being checked before PUSHing", stk);

	printf("size is %zu and capacity is %zu\n", stk->size, stk->capacity);
	
	if(stk->size < stk->capacity)
		((StkElem*)(stk->data))[stk->size++] = value;
	else 
	{
		printf("Stack %s is full\n", stk->info->name);
		CONCAT(expandStack, StkElem)(stk);
		((StkElem*)(stk->data))[stk->size++] = value;
	}

	stk->hash = hash(stk->data - sizeof(StkCanary),
					 stk->capacity * sizeof(StkElem) + 2 * sizeof(StkCanary));

	checkStack(StkElem, "Stack is being checked after PUSHing", stk);
}


bool STK_isStackOk(StkElem)(STACK(StkElem)* stk)
{
	if (hash(stk->data - sizeof(StkCanary),
			 stk->capacity * sizeof(StkElem) + 2 * sizeof(StkCanary)) != stk->hash)
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


void STK_stackDump(StkElem)(const char* result, const char* reason,
							STACK(StkElem)* stk, Location loc) 
{						
	// printf("\n%s\n", __PRETTY_FUNCTION__);

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
		"Dump called from:\n"
		"\tfunction: %s;\n"
		"\tfile: %s\n"
		"\tline: %d\n"
		"reason: %s;\n{",
		stk->info->name, result, stk, 
		stk->info->location.Function, stk->info->location.File, stk->info->location.Line,
		loc.Function, loc.File, loc.Line,
		reason
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
			"\t\tFrontCanary: 0x%llx\n",
			*((StkCanary*)(stk->data-sizeof(StkCanary))) 
		);
	#endif

	for ( size_t i = 0; i < stk->capacity; i++)
		if( ((StkElem*)( stk->data ))[i] == STK_POISON_VALUE)
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
			"\t\tBackCanary:  0x%llx\n", 
			*((StkCanary*)(stk->data+sizeof(StkElem)*stk->capacity))
		);
	#endif

	// fprintf(stream, "\n\t}\n\thash: %llu\n}\n============++++===============\n\n\n", stk_pointer->hash);
	_STK_DUMP
	(
		"\n\t}\n\thash: 0x%llx\n}\n============++++===============\n\n\n",
		stk->hash
	);

	dump_buff_pos = 0;
	memset(dump_buffer, 0, BUFSIZ);
}



void STK_cleanStack(StkElem)(STACK(StkElem)* stk)
{
	assert(stk && "bad stk pointer in cleanStack");

	for(size_t i = 0; i < stk->capacity; i ++)
		((StkElem*) (stk->data))[i] = STK_POISON_VALUE;

	stk->hash = hash(stk->data, stk->capacity * sizeof(StkElem));
}

#ifdef RESIZEABLE
	void STK_expandStack(StkElem)(STACK(StkElem)* stk)
	{
		
		// printf("================EXPANDING===============\n");
		pr_warn(LOG_CONSOLE, "Expanding stack \"%s\"\n", stk->info->name);
		assert(stk && "bad stk pointer in expandStack");

		stk->data -= sizeof(StkCanary);
		stk->data = (char*)realloc(stk->data, stk->capacity * sizeof(StkElem) * 2 + sizeof(StkCanary) * 2);

		assert(stk->data && "Bad reallocation while expanding");

		
		stk->capacity = stk->capacity * 2;

		*(StkCanary*)(stk->data) = CanaryValue;
		*(StkCanary*)(stk->data + sizeof(StkCanary) + stk->capacity * sizeof(StkElem)) = CanaryValue;

		stk->data += sizeof(StkCanary);

		for( size_t i = stk->size; i < stk->capacity; i++)
			((StkElem*)(stk->data))[i] = STK_POISON_VALUE;

		stk->hash = hash(stk->data, stk->capacity * sizeof(StkElem));
	}

	void STK_squeezeStack(StkElem)(STACK(StkElem)* stk)
	{
		// printf("================SQUEEZING===============\n");
		pr_warn(LOG_CONSOLE, "Squeezing stack \"%s\"\n", stk->info->name);

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