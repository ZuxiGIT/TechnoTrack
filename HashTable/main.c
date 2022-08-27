#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

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

size_t countWords(const char* txt, const char* delim)
{
    size_t words = 0;
    bool inword = false;

    for(; *txt != '\0'; txt++)
    {
        if(isalpha(*txt))
            inword = true;
        else
            for(const char* del = delim; *del != '\0'; del++)
                if(*txt == *del && inword) 
                {
                    inword = false;
                    words++;
                    break;
                }
    }

    return words;
}

#define INPUT  "../dict1.txt"
//#define INPUT  "./Readme.txt"
#define DELIMETERS  " \n\r\t=-\"+,:;/.()[]!?0123456789"

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

    hashtable_t* htab = HashTableCtor(256);

    int i = 0;
    size_t words = 0;

    printf("words: %zu\n", words = countWords(txt, DELIMETERS));

    char** wordsp = (char**)calloc(words, sizeof(char*));
    char* wordp = NULL;

    while((wordp = strsep(&txt, DELIMETERS)) != NULL && (txt != NULL))
        if(*wordp != '\0')
        {
            wordsp[i++] = wordp;
            //printf("word (%s)\n", word);
        }

    srand(time(NULL));

    for(size_t j = 0; j < words; j ++)
    {
        int a = rand() % words;
        //pr_info(LOG_CONSOLE, "adding new word %s\n", wordsp[a]);
        HashTableAddElem(htab, wordsp[a], j);
    }

    for(size_t j = 0; j < words; j ++)
        //pr_info(LOG_CONSOLE, "[j=%d] word: \"%s\", value: %d\n", j, wordsp[j], HashTableGetElemByKey(htab, wordsp[j]));
        if(HashTableGetElemByKey(htab, wordsp[j]) == INT_MAX)
            (void)0;//pr_err(LOG_CONSOLE, "Failrd to find: key \"%s\"\n", wordsp[j]);

    pr_info(LOG_CONSOLE, "Total words: %d\n", i);
    pr_info(LOG_CONSOLE, "Total hashtable size: %d bytes\n", HashTableSizeInBytes(htab));
    pr_info(LOG_CONSOLE, "Text size is: %d bytes\n", size);

    HashTableDtor(htab);
    free(txt_save);
    free(wordsp);

    return 0;
}
