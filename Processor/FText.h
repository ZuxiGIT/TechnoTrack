#ifndef FText_hpp
#define FText_hpp

#include "Libraries.h"


size_t fsize(const char* name);
char* CreateText(const char* filename, const size_t size);
int* Translate(Line* lines, size_t txt_size, int* error_code);
size_t NumOfLines(const char* text);
Line* ParseLines(char* lines, size_t size);
void FPrint(Line* ind, size_t num_of_lines, const char* output = "output.txt");

#endif /* FText_hpp */