#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <errno.h>

#include "Strings.h"
#include "File.h"
#include "logger.h"

size_t fileSize(const char* name)
{
	assert(name != NULL);

	struct stat stbuf = {};

	// errno = 0;
	stat(name, &stbuf);

	if(errno)
	{	 
		perror(__PRETTY_FUNCTION__); // change to perror
		return -1;
	}

	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		pr_err(LOG_CONSOLE, "%s: can't access %s. It is a directory", __PRETTY_FUNCTION__, name);
		return -2;
	}

	return stbuf.st_size;
}

void filePrint(Text* text)
{
	FILE* fp = fopen("output.txt", "a");
	
	assert(fp != NULL);

	for (int i  = 0; i < text->num_of_lines; i ++)
		fprintf(fp, "%s\n", text->text[i].start);

	fprintf(fp, "\n============================================\n"
				"\n============================================\n");

	fclose(fp); 
}

unsigned char* readText(const char* name, const size_t size)
{
	assert(name != NULL);
	
	if(!name)
	{
		pr_warn(LOG_CONSOLE, "No file path given\n");
		return NULL;
	}

	FILE* fp = fopen(name, "r");
	
	assert(fp != NULL);
	
	if(!fp)
	{
		pr_warn(LOG_CONSOLE, "Wrong file path\n");
		return NULL;
	}

	unsigned char* buff = (unsigned char*)calloc(size + 1, sizeof(unsigned char));
	
	assert(buff != NULL);

	buff[size] = '\0';

	size_t written_sz = fread(buff, sizeof(unsigned char), size, fp); // logging
	pr_info(LOG_CONSOLE | LOG_FILE, "Declared size (sizeof(char)): %zu Written size (sizeof(char)): %zu\n", size, written_sz);

	fclose(fp);

	return buff;
}