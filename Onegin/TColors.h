#ifndef TCOLORS_H
#define TCOLORS_H
#include <stdio.h>

/*! \file */

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

void resetColor()
{
    printf("\033[0;0m");
}

void setColor(int color)
{
    printf("\033[%dm", color);
}

#endif /* TCOLORS_H */