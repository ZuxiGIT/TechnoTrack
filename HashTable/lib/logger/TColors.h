#ifndef TCOLORS_H
#define TCOLORS_H

#include <stdio.h>

enum  {
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN,
    FG_YELLOW,
    FG_BLUE,
    FG_MAGENTA,
    FG_CYAN,
    FG_WHITE
};

enum  {
    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN,
    BG_YELLOW,
    BG_BLUE,
    BG_MAGENTA,
    BG_CYAN,
    BG_WHITE
};

static int resetColor(char* buff)
{
    if(buff == NULL)
    {
        printf("\033[0m");
        fflush(stdout);
        return 0;
    }
    else
        return sprintf(buff, "\033[0m");

}

static int setColor(char* buff, int color)
{
    if(buff == NULL)
    {
        printf("\033[%dm", color);
        fflush(stdout);
        return 0;
    }
    else
        return sprintf(buff, "\033[%dm", color);
}

#endif /* TCOLORS_H */
