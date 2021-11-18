#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <errno.h>
#include <wchar.h>
#include <stdlib.h>

#include "Strings.h"
#include "File.h"
#include "../logger/logger.h"

int fileSize(const char* name)
{
	assert(name != NULL);

	struct stat stbuf = {};

	errno = 0;
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
    mblen(NULL, 0);

    pr_info(LOG_CONSOLE, "Beginning readText\n");
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

    //fwide(fp, 1);
    //freopen(NULL, "a+", stdout);
    if(fwide(stdout, 0));
    printf("!!!!!!");
    printf("!!!%d!!!", fwide(stdout, 0));
    //freopen(NULL, "a+", stdout);
    printf("fwide(fp, 0) = %d\n", fwide(fp, 0));
    

	unsigned char* buff = (unsigned char*)calloc(size + 1, sizeof(unsigned char));
	
	assert(buff != NULL);

	buff[size] = '\0';


	int written_sz = fread(buff, sizeof(unsigned char), size, fp);// logging
    printf("Length is %ld\n", mbstowcs(NULL, buff, 0));
    wchar_t* txt = (wchar_t*)calloc(mbstowcs(NULL, buff, 0) + 1, sizeof(wchar_t));

    mbstowcs(txt, buff, mbstowcs(NULL, buff, 0));
	pr_info(LOG_CONSOLE | LOG_FILE, "Declared size (sizeof(char)): %zu Written size (sizeof(char)): %zu\n", size, written_sz);

	fclose(fp);

    freopen(NULL, "w", stdout);
    wprintf(L"Char: %lc\n", *txt);
    wprintf(L"TEXT\n%ls\n", txt);
    freopen(NULL, "w", stdout);

	return txt;
}
