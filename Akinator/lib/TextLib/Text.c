#include "Text.h"
#include "../logger/logger.h"
#include "File.h"
#include <malloc.h>
#include <assert.h>
#include <wchar.h>

// static unsigned char* txt = NULL;

Text* text_init(const char* path)
{
    int sz = fileSize(path);
    
    if(sz < 0)
    {
        pr_err(LOG_CONSOLE, "Wrong file path\n");
        return NULL;
    }

	pr_info(LOG_CONSOLE, "sz is %zu\n", sz);

    wchar_t* txt = readText(path, sz);
	int num_of_lines = numberOfLines(txt);
    
	pr_info(LOG_CONSOLE, "Total number of lines is %d\n", num_of_lines);
	// pr_info(LOG_CONSOLE, "read :\n%s\n", txt);

	Line* ind = parseText(txt, num_of_lines);
    
    free(txt);

    Text* res = (Text*)calloc(1, sizeof(Text));
    
    res->text = ind;
    res->num_of_lines = num_of_lines;

    
    return res;
}

void text_free(Text* text)
{
    for(int i = 0; i < text->num_of_lines; i++)
        free(text->text[i].start);

    // free(text->text->start);

    free(text->text);
    free(text);
}


void fprintText(Text* text)
{
    fprintTextWithDelimeters(text, '\n');
}

void fprintTextBinary(Text* input)
{
    assert(input != NULL);

	FILE* fp = fopen("output.txt", "wb");
	
	assert(fp != NULL);
    fwide(fp, 1);

////------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for (int i  = 0; i < input->num_of_lines; i ++)
	{
        wchar_t format[10] = {};
        swprintf(format, 10, L"%%%ds", input->text[i].length);
        wprintf(L"----------->format is %ls\n", format);
        if(input->text[i].start)
    	    fwprintf(fp, format, input->text[i].start);
    }
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11


	// fprintf(fp, "\n============================================\n"
				// "\n============================================\n");

	fclose(fp); 

}

void fprintTextWithDelimeters(Text* input, char delim)
{
	FILE* fp = fopen("output.txt", "a");
	
	assert(fp != NULL);
    fwide(fp, 1);

	for (int i  = 0; i < input->num_of_lines; i ++)
		fwprintf(fp, L"%ls%lc", input->text[i].start, btowc(delim));

	// fprintf(fp, "\n============================================\n"
				// "\n============================================\n");

	fclose(fp); 
}


void printText(Text* src)
{
    printTextWithDelimeters(src, '\n');
}


void printTextWithDelimeters(Text* src, char delim)
{
    assert(src);

    for(int i = 0; i < src->num_of_lines; i++)
        wprintf(L"%ls%lc", src->text[i].start, btowc(delim));
}


int textBinaryLength(Text* txt)
{
    int res = 0;
    for(int i = 0; i < txt->num_of_lines; i++)
        res += txt->text[i].length;
    
    return res;
}
