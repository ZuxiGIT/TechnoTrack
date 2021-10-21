#include <stdio.h>

int main(int argc, char const *argv[])
{
	char u[10] = {};
	u[3] = 'a';
	u[0] = 'c';
	printf("%s\n", u);
	return 0;	
}
