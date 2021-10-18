#ifndef Print_hpp
#define Print_hpp
#include <stddef.h>



int Print_ll(char* dest, size_t maxlen, long long ValueL);

int Print_double(char* dest, size_t maxlen, double ValueD);

int Print_char(char* dest, size_t maxlen, char ValueC);

int Print_ull(char* dest, size_t maxlen, unsigned long long ValueU);

int Print_int(char* dest, size_t maxlen, int ValueI);

int Print_u(char* dest, size_t maxlen, unsigned ValueU);

int Print_uc(char* dest, size_t maxlen, unsigned char ValueUC);

#endif /* Print_hpp */