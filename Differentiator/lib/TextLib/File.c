#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <errno.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>

#include "Strings.h"
#include "File.h"
#include "../logger/logger.h"

void wideCharLocaleInit()
{
    setlocale(LC_ALL, "");
    //printf("Starrt");
    //abort();
    //exit(-1);
    mblen(NULL, 0);
}

FILE* myfreopen()
{
    ;
}
int fileSize(const char* name)
{
	assert(name != NULL);

	struct stat stbuf = {};

	errno = 0;
	stat(name, &stbuf);

	if(errno)
	{	 
        perror(__PRETTY_FUNCTION__); // change to perror
        errno = 0;
        return -1;
	}

	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		pr_err(LOG_CONSOLE, "%s: can't access %s. It is a directory", __PRETTY_FUNCTION__, name);
		return -2;
	}

	return stbuf.st_size;
}

char* readText(const char* name, const size_t size)
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

	int written_sz = fread(buff, sizeof(unsigned char), size, fp);// logging

	pr_info(LOG_CONSOLE | LOG_FILE, "Declared size (sizeof(char)): %d \
                                     Read size (sizeof(char)): %d",
                                     size, written_sz);

    free(buff);
	fclose(fp);

	return buff;
}

wchar_t* wreadText(const char* name, const size_t size)
{
    mblen(NULL, 0);

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
#if 0
    fwide(fp, 1);
    freopen(NULL, "a+", stdout);
    if(fwide(stdout, 0));
    printf("!!!!!!");
    printf("!!!%d!!!", fwide(stdout, 0));
    freopen(NULL, "a+", stdout);
    printf("fwide(fp, 0) = %d\n", fwide(fp, 0));
#endif 

	unsigned char* buff = (unsigned char*)calloc(size + 1, sizeof(unsigned char));
	
	assert(buff != NULL);

	buff[size] = '\0';

	int written_sz = fread(buff, sizeof(unsigned char), size, fp);// logging

    int num_of_chars = mbstowcs(NULL, buff, 0);

    //printf("Length is %ld\n", num_of_chars);

    wchar_t* txt = (wchar_t*)calloc(num_of_chars + 1, sizeof(wchar_t));

    mbstowcs(txt, buff, num_of_chars);

	pr_info(LOG_CONSOLE | LOG_FILE, "Declared size (sizeof(char)): %d Read size (sizeof(char)): %d Numbers of chars(sizeof(wchar_t)) %d\n", size, written_sz, num_of_chars);

    free(buff);
	fclose(fp);

    //freopen(NULL, "w", stdout);
    //wprintf(L"Char: %lc\n", *txt);
    //wprintf(L"TEXT\n%ls\n", txt);
    //freopen(NULL, "w", stdout);


	return txt;
}
