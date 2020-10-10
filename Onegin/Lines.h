#ifndef Lines_hpp
#define Lines_hpp

#include <assert.h>
#include <stdio.h>
#include <malloc.h>

typedef struct{ unsigned char* start; unsigned char* finish; int length;} Line;
typedef unsigned char uc;

int isletter(unsigned char symbol);
int NumOfLines(const unsigned char* text);
Line* ParseLines(unsigned char* lines, int size);

#endif /* Lines_hpp */