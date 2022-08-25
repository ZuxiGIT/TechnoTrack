#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "hashtable.h"
#include "./lib/logger/logger.h"

size_t fileSize(const char* name)
{
	assert(name != NULL);

	struct stat stbuf = {};

	stat(name, &stbuf);

	if(errno)
	{	 
		perror(__PRETTY_FUNCTION__); 
		return -1;
	}

	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		pr_err(LOG_CONSOLE, "%s: can't access %s. It is a directory", __PRETTY_FUNCTION__, name);
		return -2;
	}

	return stbuf.st_size;
}

const char* INPUT = "../input_text.txt";

int main()
{
    int size = fileSize(INPUT);
    FILE* fp = fopen(INPUT, "r");
    char* txt = (char*)calloc(size + 1, sizeof(char));

    if(!txt)
    {
        errno = 0;
        pr_err(LOG_CONSOLE, "Cannot allocate mem for text");
        return -1;
    }

    fread(txt, 1, size, fp);
    fclose(fp);
    printf("text size is: %d bytes\n", size);
    getchar();

    char* txt_save = txt;
    char* word = NULL;

    hashtable_t* htab = HashTableCtor(256);

    int i = 0;

    while((word = strsep(&txt, " =:;/-.[]\n\r")) != NULL && (txt != NULL))
        if(*word != '\0')
        {
            i++;
            if(HashTableAddElem(htab, word, i) < 0)
            {
                printf("failure\n");
                exit(-1);
            }
        }

    
    printf("Total words: %d\n", i);
    printf("Total size: %d bytes\n", HashTableSizeInBytes(htab));

    HashTableDtor(htab);
    free(txt_save);

    return 0;
}
