#include "logger.h"
#include "TColors.h"
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static FILE* log_file = NULL;
static char filepath[256] = "log.txt";
static char buff[BUFSIZ] = {};
static int buff_pos = 0;



void log_set_path(const char* path)
{
    memccpy(filepath, path, '\0', sizeof(filepath));
}

static void _checkErrors(const char* str);

void log_close()
{
    fclose(log_file);
}


void log_init(const char* path)
{
    if (!path)
    {
        log_file = fopen(filepath, "a");

        assert(log_file != NULL);
        
        if(errno || !log_file)
        {
            perror(__PRETTY_FUNCTION__);
            errno = 0;
        }
    }
    else
    {
        log_file = fopen(path, "a");

        if(errno || !log_file)
        {
            perror(__PRETTY_FUNCTION__);
            errno = 0;
        }
    }
}

static void _pr_warn(int log_level)
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
    if (log_level == LOG_WARN)
    {
        ret = sprintf(buff + buff_pos, "WARN: ");
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

    _pr_warn(log_level);
    
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
        fflush(stdout);
        
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


static void _checkErrors(const char* str)
{
    if(errno)
    {
        perror(str);
        errno = 0;
    }
}