#include "Strings.h"
#include "Sort.h"
#include <string.h>

void swap(Line* a, Line* b);
void generalizedSwap(void* a, void* b, int size);

void selectionSort(Text* text, int (*comp)(const void* a, const void* b))
{
	int max_index = 0;
    if (text->num_of_lines >= 2)
    {
        for (int i = 0; i < text->num_of_lines; i++)
        {
            for (int j = 0; j < text->num_of_lines - i; j ++)
                if (comp(&text->text[j], &text->text[max_index]) > 0)
                    max_index = j;
            
            swap(&text->text[max_index], &text->text[text->num_of_lines - i - 1]);

            max_index = 0;
        }
    }
}

void generalizedSelectionSort(void* buff, int size, int elem_size, int(*comp)(const void* a, const void* b))
{
	int max_index = 0;
    
    
    if (size >=2)
    {
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size - i; j++)
                if(comp((unsigned char*)buff + j*elem_size, (unsigned char*)buff + max_index*elem_size) > 0)
                    max_index = j;
            
            generalizedSwap((unsigned char*)buff + max_index*elem_size, (unsigned char*)buff + (size - i - 1)*elem_size, elem_size);

            max_index = 0;
        }
    }
}

void generalizedSwap(void* a, void* b, int size)
{
    unsigned char temp_buff[size];

    memcpy(temp_buff, a, size);
    memcpy(a, b, size);
    memcpy(b, temp_buff, size);

}
void swap(Line* a, Line* b)
{
	Line temp = *a;
	*a = *b;
	*b = temp;
}