#ifndef Lines_hpp
#define Lines_hpp

#include "Libraries.h"

unsigned char toLower(unsigned char symbol); 
int isletter(unsigned char symbol);
int NumOfLines(const unsigned char* text);
Line* ParseLines(unsigned char* lines, int size);

#endif /* Lines_hpp */