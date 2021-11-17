#include "Text.h"
#include "logger.h"
#include "File.h"
#include <malloc.h>

static unsigned char* txt = NULL;

Text* text_init(const char* path)
{
    size_t sz = fileSize(path);
	pr_info(LOG_CONSOLE, "sz is %zu\n", sz);

	txt = readText(path, sz);
	int num_of_lines = numberOfLines(txt);
    
	pr_info(LOG_CONSOLE, "Total number of lines is %d\n", num_of_lines);
	// pr_info(LOG_CONSOLE, "read :\n%s\n", txt);

	Line* ind = parseText(txt, num_of_lines);
    
    // free(txt);

    Text* res = (Text*)calloc(1, sizeof(Text));
    
    res->text = ind;
    res->num_of_lines = num_of_lines;

    return res;
}

void text_free(Text* text)
{
    free(txt);
    free(text->text);
}