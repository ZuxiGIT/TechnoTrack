#ifndef Sort_hpp
#define Sort_hpp

#include "Libraries.h"

void SelectionSort(Line* ind, int size, int (*comp)(const void* a, const void* b));
void Swap(Line* a, Line* b);

#endif /* Sort_hpp */