#include "Strings.h"
#include "File.h"
#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>

size_t fileSize(const char* name)
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

void filePrint(Line* ind, int num_of_lines)
{
	FILE* fp = fopen("output.txt", "a");
	
	assert(fp != NULL);

	for (int i  = 0; i < num_of_lines; i ++)
		fprintf(fp, "%s\n", ind[i].start);
	fprintf(fp, "\n============================================\n"
				"\n============================================\n");

	fclose(fp); 
}

unsigned char* readText(const char* name, const size_t size)
{
	assert(name != NULL);

	FILE* fp = fopen(name, "r");
	
	assert(fp != NULL);
	
	unsigned char* buff = (unsigned char*)calloc(size + 1, sizeof(unsigned char));
	
	assert(buff != NULL);

	buff[size] = '\0';

	size_t written_sz = fread(buff, sizeof(unsigned char), size, fp);
	printf("Declared size (sizeof(char)): %zu Written size (sizeof(char)): %zu\n", size, written_sz);

	fclose(fp);

	return buff;
}