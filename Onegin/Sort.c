#include "Sort.h"

void SelectionSort(Line* ind, int size, int (*comp)(const void* a, const void* b))
{
	int max_index = 0;
    if (size >= 2)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size - i; j ++)
                if (comp(&ind[j], &ind[max_index]) > 0)
                    max_index = j;
            
            Swap(&ind[max_index], &ind[size - i - 1]);

            max_index = 0;
        }
    }
}

void Swap(Line* a, Line* b)
{
	Line temp = *a;
	*a = *b;
	*b = temp;
}