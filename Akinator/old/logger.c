#include "logger.h"
#include "TColors.h"
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>


static FILE* log_file = NULL;
static char buff[BUFSIZ] = {};
static int buff_pos = 0;


void _checkErrors(const char* str);

void log_close()
{
    fclose(log_file);
}


void log_init(const char* path)
{
    if (!path)
    {
        log_file = fopen("log.txt", "a");

        assert(log_file != NULL);
        
        if(errno || !log_file)
        {
            perror("LOG_INIT: ");
            errno = 0;
        }
    }
    else
    {
        log_file = fopen(path, "a");

        if(errno || !log_file)
        {
            perror("LOG_INIT: ");
            errno = 0;
        }
    }
}

void pr_warn(int log_level)
{
    int ret = 0;
    if (log_level == LOG_ERR)
    {
        ret = sprintf(buff + buff_pos, "ERROR: ");
        buff_pos += ret;
    }
    if (log_level == LOG_INFO)
    {
        ret = sprintf(buff + buff_pos, "INFO: ");
        buff_pos += ret;
    }
}

void pr_log_level(int log_level, int dest, const char* fmt, ...)
{
    buff_pos = 0;
    
    if(!log_file)
    {
        log_init(NULL);
    }

    pr_warn(log_level);
    
    va_list params;
    va_start(params, fmt);

    errno = 0;
    int ret = 0;
    
    ret = vsnprintf(buff + buff_pos, BUFSIZ - buff_pos, fmt, params);
    
    buff_pos += ret;

    _checkErrors("Local buffer writing: ");

    va_end(params);

    if (!log_file)
    {
        setColor(FG_RED);
        printf("ERROR: Can not use logging\n");
        resetColor();
        return;
    }

    assert(log_file != NULL);
    
    if (!!(dest & LOG_CONSOLE))
    {
        if (log_level == LOG_INFO)
            setColor(FG_GREEN);
        else if (log_level == LOG_WARN)
            setColor(FG_YELLOW);
        else if (log_level == LOG_ERR)
            setColor(FG_RED);
        
        fwrite(buff, sizeof(char), buff_pos, stdout);
        _checkErrors("Console logging: ");
        resetColor();
    }
    
    if (!!(dest & LOG_FILE))
    {
        fwrite(buff, sizeof(char), buff_pos, log_file);
        _checkErrors("File writing: ");
    }

    while(buff_pos) buff[--buff_pos] = '\0';
}


void _checkErrors(const char* str)
{
    if(errno)
    {
        perror(str);
        errno = 0;
    }
}