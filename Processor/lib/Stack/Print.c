#include "Print.h"
#include <stdio.h>


int Print_ll(char* dest, size_t maxlen, long long ValueL)
{
    return snprintf(dest, maxlen, " %lld\n",ValueL);
}

int Print_double(char* dest, size_t maxlen, double ValueD)
{
    return snprintf(dest, maxlen, " %lf\n", ValueD);
}

int Print_char(char* dest, size_t maxlen, char ValueC)
{
    return snprintf(dest, maxlen, " %c\n", ValueC);
}

int Print_ull(char* dest, size_t maxlen, unsigned long long ValueU)
{
    return snprintf(dest, maxlen, " %llu\n", ValueU);
}

int Print_int(char* dest, size_t maxlen, int ValueI)
{
    return snprintf(dest, maxlen, " %d\n", ValueI);
}

int Print_u(char* dest, size_t maxlen, unsigned ValueU)
{
    return snprintf(dest, maxlen, " %u\n", ValueU);
}

int Print_uc(char* dest, size_t maxlen, unsigned char ValueUC)
{
    return snprintf(dest, maxlen, " %hhu\n", ValueUC);
}