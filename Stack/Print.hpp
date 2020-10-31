#ifndef Print_hpp
#define Print_hpp

#include <stdio.h>



void Print(FILE* fp, long long ValueL);                    

void Print(FILE* fp, double ValueD);

void Print(FILE* fp, char ValueC);

void Print(FILE* fp, unsigned long long ValueU);

void Print(FILE* fp, int ValueI);

void Print(FILE* fp, unsigned ValueU);

void Print(FILE* fp, unsigned char ValueUC);

#endif /* Print_hpp */