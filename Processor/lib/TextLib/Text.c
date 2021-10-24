#include "Text.h"
#include "../logger/logger.h"
#include "File.h"
#include <malloc.h>
#include <assert.h>

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

	unsigned char* txt = readText(path, sz);
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
	FILE* fp = fopen("output.txt", "wb");
	
	assert(fp != NULL);

////------____!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for (int i  = 0; i < input->num_of_lines; i ++)
	{
        char format[10] = {};
        snprintf(format, 10, "%%%ds", input->text[i].length);
        // printf("----------->format is %s\n", format);
    	fprintf(fp, format, input->text[i].start);
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

	for (int i  = 0; i < input->num_of_lines; i ++)
		fprintf(fp, "%s%c", input->text[i].start, delim);

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
        printf("%s%c", src->text[i].start, delim);
}