#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <errno.h>

#include "Strings.h"
#include "File.h"
#include "../logger/logger.h"

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


wchar_t* readText(const char* name, const size_t size)
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

    fwide(fp, 1);

	wchar_t* buff = (wchar_t*)calloc(size + 1, sizeof(wchar_t));
	
	assert(buff != NULL);

	buff[size] = L'\0';

	size_t written_sz = fread(buff, sizeof(unsigned char), size, fp); // logging
	pr_info(LOG_CONSOLE | LOG_FILE, "Declared size (sizeof(char)): %zu Written size (sizeof(char)): %zu\n", size, written_sz);

	fclose(fp);

	return buff;
}
