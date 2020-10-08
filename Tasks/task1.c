#include <stdio.h>
#include <assert.h>
#include <string.h>



size_t StrLen(const char* p)
{
	assert(p != NULL);

	size_t i = 0;
	
	while(*p++) i++;
	
	return i;
}

void StrnCpy(char* destination, char* source, size_t num)
{
	assert(destination != NULL);
	assert(source 	   != NULL);

	size_t srcLen = StrLen(source);
	size_t i = 0;

	for(i; (i < srcLen) && (i < num); i ++)
		destination[i] = source[i];

	if(srcLen < num)
		for(i; i < num; i++)
			destination[i] = '\0';
}

char* StrnCat(char* dest, const char* src, size_t n)
{
	assert(dest != NULL);
	assert(src  != NULL);

	size_t destLen = StrLen(dest);

	for(int i = 0; i < n; i ++)
		dest[destLen + i] = src[i];
	dest[destLen + n] = '\0';
	
	return dest;
}

char* StrStr(const char * string1, const char * string2)
{
	assert(string1 != NULL);
	assert(string2 != NULL);

	size_t str1Len = StrLen(string1);
	size_t str2Len = StrLen(string2);


	for(int i = 0; i < str1Len; i ++)
		if (string1[i] == string2[0])
			for(int j = 0; j < str2Len; j ++)
			{
				if (string1[i+j] != string2[j])
					break;
				if (j == (str2Len - 1))
					return ((char*)string1 + i);
			}
}

char* StrTok(char * string, const char * delim)
{
	static char* last = NULL;
	if (string) last = string;
	if (last == NULL || *last == '\0') return NULL;
	
	char* c = last;	
	while(strchr(delim, *c)) c++;
	
	if (*c == '\0') return NULL;
	
	char* start = c;
	while(*c && (strchr(delim, *c) == 0)) c++;
	
	if(*c == '\0')
	{
		last = c;
		return start;
	}
	
	*c = '\0';
	last = c+1;
	
	return start;
}

int main()
{
	char* p = "abc";
	char* d = StrStr(p, "c");
	printf("p = %p\nd = %p\n", p, d);
	
	char ex[10] = {};
	StrnCpy(ex, p, 3);
	printf("%zu\n%s\n", StrLen(p), ex);
	
	char* s = StrnCat(ex, p, 2);
	printf("%s\n", s); 


	char str[] = "Особенности национальной рыбалки - художественный, комедийный фильм.";
	char * pch = StrTok (str," ,.-");
	while (pch != NULL)
	{
		printf("%s\n", pch);
		pch = StrTok (NULL, " ,.-");
	}
}