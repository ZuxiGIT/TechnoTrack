#include "FText.h"
#include "CPUcommands.h"
//--------------------------FILE------------------------------

/**
	Calculate the file's size

	\param[in] name - the file name

	\return the file's size
*/

size_t fsize(const char* name)
{
	assert(name != NULL);

	struct stat stbuf = {};

	if (stat(name, &stbuf) == -1)
	{	
		fprintf(stderr, "fsize: can't access %s\n", name);
		return 0;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		fprintf(stderr, "fsize: can't access %s. It is a directory", name);
		return 0;
	}
	return stbuf.st_size;
}

/**
	Read text from the file

	\param[in] name - the filename
	\param[in] size - the file's size

	\return pointer to the array of unsigned char
*/

char* CreateText(const char* name, const size_t size)
{
	assert(name != NULL);

	FILE* fp = fopen(name, "r");
	
	assert(fp != NULL);
	
	char* buff = (char*)calloc(size + 1, sizeof(char));
	
	assert(buff != NULL);

	buff[size] = '\0';

	size_t written_sz = fread(buff, sizeof(char), size, fp);
	printf("Declared size (sizeof(char)): %zu Written size (sizeof(char)): %zu\n", size, written_sz);

	fclose(fp);

	return buff;
}

/**
	Writes the results to a file

	\param[in] index  - array of structures "Line"
	\param[in] number - number of lines
	
*/
void FPrint(Line* ind, size_t num_of_lines, const char* output)
{
	FILE* fp = fopen(output, "w");
	
	assert(fp != NULL);

	for (size_t i  = 0; i < num_of_lines; i ++)
		{
			if(ind[i].length == 0)
				continue;
			fprintf(fp, "%s\n", ind[i].start);
		}

	fclose(fp); 
}

/** 
	Count lines

	\param[in] text - pointer to the array of unsigned char

	\return number of lines
*/
size_t NumOfLines(const char* text)
{
	assert(text != NULL);

	size_t i = 0;
	
	while (*text != '\0')
	{
		if (*text == '\n')
			i++;
		text++;
	}
	i++;
	return i;
}

/**
	Parse lines from the array of unsigned char

	\param[in] text - pointer to the array of unsigned char
	\param[in] size - number of lines

	\return pointer to the array of Line
*/

Line* ParseLines(char* lines, size_t size)
{
	assert(lines != NULL);

	Line* index = (Line*)calloc(size, sizeof(Line));

	assert(index != NULL);
	
	char* begin 		= lines;
	char* end   		= begin;
	char* temp_pointer 	= NULL;

	for (size_t i = 0; i < size; i ++)
	{
		while (strchr(";\n" , *end) == NULL) 
			end++;

		temp_pointer = end;
		
		if (*temp_pointer == ';')
			while (strchr("\n" , *temp_pointer) == NULL) temp_pointer++;

		*end = '\0';

		index[i].start 	= begin;
		index[i].finish = end;
		index[i].length = (end - begin);
		//printf("string: %s\n", begin);

		begin 	= temp_pointer + 1;
		end 	= begin;	
	}

	return index;
}

size_t Num_of_args(char* ind)
{
	char* word = strtok(ind, " ");
	size_t i = 0;
	while(word != NULL)	{ word = strtok(NULL, " "); i++;}
	return i - 1;
}

int* Translate(Line* lines, size_t num_of_lines, int* error_code)
{
	size_t num_of_cmd = sizeof(CPU_commands)/sizeof(CPUCMD);
	int sum;

	for (size_t i = 0; i < num_of_lines; i ++)
		sum += lines[i].length;

	int* output 	= (int*)calloc(sum, sizeof(int));
	char* string 	= (char*)calloc(80, sizeof(char));
	char* func_name	= (char*)calloc(20, sizeof(char));

	size_t num_args = 0;

	for (size_t i = 0; i < num_of_lines; i ++)
	{
		memcpy(string, lines[i].start, lines[i].length);
		printf("%s\n", string);
		
		num_args = Num_of_args(string);
		memcpy(string, lines[i].start, lines[i].length);
		
		memcpy(func_name, string, 20);
		func_name = strtok(func_name, " ");
		

		for(size_t j = 0; j < num_of_cmd; j ++)
			if (strcmp(CPU_commands[i].asm_code, func_name) == 0)
			{
				if (num_args != CPU_commands[i].num_of_args)
					fprintf(stderr, "%s needs %zu arguments ( %zu were given)\n", func_name, CPU_commands[i].num_of_args, num_args);
			}
			else
				fprintf(stderr, "%s is not a command\n", func_name);
		memset(func_name, '\0', 20);
		//sscanf(lines[i].start, "%s", string);
		memset(string, '\0', lines[i].length);
	}
	return output;
}