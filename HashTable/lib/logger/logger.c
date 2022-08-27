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

#define _checkErrors(str)\
{\
    char error[128] = {};\
    sprintf(error, "%s:%d %s", __FILE__, __LINE__, str);\
    __checkErrors(error);\
}

void log_set_path(const char* path)
{
    memccpy(filepath, path, '\0', sizeof(filepath));
}

static void __checkErrors(const char* str);

void log_close()
{
    if(log_file)
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
    _checkErrors("Before Console logging: ");
    buff_pos = 0;
    
    if(!log_file)
    {
        log_init(NULL);
    }
    
    if(!!(dest & LOG_CONSOLE_STDERR))
    {
        if (log_level == LOG_INFO)
            buff_pos += setColor(buff + buff_pos, FG_GREEN);
        else if (log_level == LOG_WARN)
            buff_pos += setColor(buff + buff_pos, FG_YELLOW);
        else if (log_level == LOG_ERR)
            buff_pos += setColor(buff + buff_pos, FG_RED);
    }

    _pr_warn(log_level);
    
    if(!!(dest & LOG_CONSOLE_STDERR))
        buff_pos += resetColor(buff + buff_pos);

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
        //setColor(NULL, FG_RED);
        fprintf(stderr, "ERROR: Can not use file logging\n");
        //resetColor(NULL);
        return;
    }

    assert(log_file != NULL);
    
    if ((dest & LOG_CONSOLE) == LOG_CONSOLE)
    {
        (void)!freopen(NULL, "w", stdout);
        fwrite(buff, sizeof(char), buff_pos, stdout);
        (void)!freopen(NULL, "w", stdout);
    }

    if((dest & LOG_STDERR) == LOG_STDERR)
    {
        (void)!freopen(NULL, "w", stderr);
        //fprintf(stderr, "----->");
        fwrite(buff, sizeof(char), buff_pos, stderr);
        (void)!freopen(NULL, "w", stderr);
    }

    fflush(stdout);

    _checkErrors("After Console logging: ");
    
    
    if (!!(dest & LOG_FILE))
    {
        fwrite(buff, sizeof(char), buff_pos, log_file);
        _checkErrors("File writing: ");
    }

    while(buff_pos) buff[--buff_pos] = '\0';
}


static void __checkErrors(const char* str)
{
    if(errno)
    {
        //setColor(FG_RED);
        perror(str);
        //resetColor();
        errno = 0;
    }
}
