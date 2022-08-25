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
    

    // +1 for '\0' byte
    char* txt = (char*)calloc(size + 1, sizeof(char));

    if(!txt)
    {
        errno = 0;
        pr_err(LOG_CONSOLE, "Cannot allocate mem for text");
        return -1;
    }

    (void)!fread(txt, sizeof(char), size, fp);
    fclose(fp);
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
                pr_err(LOG_CONSOLE, "adding an element failure\n");
                exit(-1);
            }
        }

    
    pr_info(LOG_CONSOLE, "Total words: %d\n", i);
    pr_info(LOG_CONSOLE, "Total hashtable size: %d bytes\n", HashTableSizeInBytes(htab));
    pr_info(LOG_CONSOLE, "Text size is: %d bytes\n", size);

    HashTableDtor(htab);
    free(txt_save);

    return 0;
}
