#include "filesystem.h"
#include "logger.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

static void _checkErrors(const char* str)
{
    if(errno)
    {
        perror(str);
        errno = 0;
    }
}

int createDir(const char* pathname)
{
    if(mkdir(pathname, 777))
    {
        pr_warn(LOG_CONSOLE, "failed to create directory");
        _checkErrors("Creating directory: ");
        return -1;
    }
    return 0;
}