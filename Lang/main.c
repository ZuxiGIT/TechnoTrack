#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>


#define REQUIRE(req, got)\
    if(*s == req) s++;\
    else \
    { \
        fprintf(stderr, "ERROR: bad string, got %c, expected %c\n", got, req);\
        exit(1);\
    }

void SyntaxError(const char* format, ...)
{
   va_list params;
   va_start(params, format);

   vprintf(format, params);

   va_end(params);
}

int GetG(const char* str);
int GetE();
int GetT();
int GetP();
int GetN();


const char* s = NULL;

int GetG(const char* str)
{
    s = str;
    int val = GetE();
    REQUIRE('$', *s);
    return val;
}

int GetE()
{
    int val = GetT();
    while(*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        int val2 = GetT();
        if(op == '+') val += val2;
        else val -= val2;
    }
    return val;
}

int GetT()
{
    int val = GetP();
    while(*s == '*' || *s == '/')
    {
        char op = *s;
        s++;
        int val2 = GetP();
        if(op == '*')   val *= val2;
        else    val /= val2;
    }
    return val;
}

int GetP()
{
    int val= 0;
    if(*s == '(')
    {
        s++;
        val = GetE();
        REQUIRE(')', *s);
    }
    else
        val = GetN();
    return val;
}

int GetN()
{
    int val = 0;
    const char* begin = s;
    while(isdigit(*s))
    {
        val = val*10 + (*s - '0');
        s++;
    }

    if(begin == s)
        SyntaxError("Error: wrong number (%c)\n", *s);

    return val;
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Error: no input\n");
        exit(1);
    }

    fprintf(stdout, "Result is: %d\n", GetG(argv[1]));
    return 0;
}
