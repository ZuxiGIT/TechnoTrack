#include "Print.hpp"



void Print(FILE* fp, long long ValueL)
{
    fprintf(fp, " %lld\n",ValueL);
}

void Print(FILE* fp, double ValueD)
{
    fprintf(fp, " %lg\n", ValueD);
}

void Print(FILE* fp, char ValueC)
{
    fprintf(fp, " %c\n", ValueC);
}

void Print(FILE* fp, unsigned long long ValueU)
{
    fprintf(fp, " %llu\n", ValueU);
}

void Print(FILE* fp, int ValueI)
{
    fprintf(fp, " %d\n", ValueI);
}

void Print(FILE* fp, unsigned ValueU)
{
    fprintf(fp, " %u\n", ValueU);
}

void Print(FILE* fp, unsigned char ValueUC)
{
    fprintf(fp, " %u\n", ValueUC);
}