#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

/*
enum TYPE
{
	OPERATOR,
	VARIABLE,
	FUNCTION,
	CONSTANT
};

typedef union 
{
	char str[8] = {0}; 
	double num;
} Value; 

struct Node
{
	TYPE 	type;
	char 	priority;
	Value 	data;
	Node* 	left 	= NULL;
	Node* 	right	= NULL;	
	Node* 	parent 	= NULL;
};

Node* CreateNode(TYPE tp = VARIABLE, Value val = {0}, Node* lft = NULL, Node* rght = NULL)
{
	Node* res 	= (Node*)calloc(1, sizeof(Node));
	
	res->type 	= tp;
	res->data 	= val;
	res->left 	= lft;
	res->right 	= rght;

	return res;
}

	Calculate the file's size

	\param[in] name - the file name

	\return the file's size


size_t fsize(const char* name)
{
	assert(name != NULL);

	struct stat stbuf = {};

	if (stat(name, &stbuf) == -1)
	{	
		fprintf(stderr, "fsize: can't access %s\n", name);
		return 0;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		fprintf(stderr, "fsize: can't access %s. It is a directory", name);
		return 0;
	}
	return stbuf.st_size;
}

unsigned char* Preprocess(unsigned char* txt)
{

	size_t len = 0;
	unsigned char* tmp = txt;
	
	while(*tmp++) len++;
	unsigned char* res = (unsigned char*)calloc(len, sizeof(char));
	
	int k = 0;

	for(size_t i = 0; i < len; i++)
	{
		if(txt[i] == '\n' || txt[i] == ' ')
			continue;
		res[k] = txt[i];
		k++;
	}
	return res;

}

/**
	Read text from the file

	\param[in] name - the filename
	\param[in] size - the file's size

	\return pointer to the array of unsigned char


unsigned char* TextFromFile(const char* name, const size_t size)
{
	assert(name != NULL);

	FILE* fp = fopen(name, "r");
	
	assert(fp != NULL);
	
	unsigned char* buff = (unsigned char*)calloc(size + 1, sizeof(unsigned char));
	
	assert(buff != NULL);

	buff[size] = '\0';

	size_t written_sz = fread(buff, sizeof(unsigned char), size, fp);
	printf("Declared size (sizeof(char)): %zu Written size (sizeof(char)): %zu\n", size, written_sz);

	fclose(fp);

	return Preprocess(buff);
}




Node* ReadTreeFrom(unsigned char* text)
{
	assert(text);

	if(*text != '(') 
		return NULL;

	*text++;

	bool isclosed = false;

	Node* res = (Node*)calloc(1, sizeof(Node));
	assert(res && "res after calloc in ReadTreeFrom");

	Node* tmp = (Node*)calloc(1, sizeof(Node));
	assert(res && "tmp after calloc in ReadTreeFrom");	 
	
	tmp->parent = tmp;	
	res = tmp;


	while(*text)
	{
		if(*text == '(')
			if(!isclosed)
			{
				Node* another_temp  	= CreateNode();
				tmp->left 				= another_temp;
				another_temp->parent 	= tmp;
				tmp 					= another_temp;
				
				text++;
			}
			else
			{
				Node* another_temp  	= CreateNode();
				tmp->right 				= another_temp;
				another_temp->parent 	= tmp;
				tmp 					= another_temp;
				
				isclosed = false;
				text++; 
			}
		else
			if(*text == ')')
			{
				tmp = tmp -> parent;
				
				isclosed = true;
				text++;
			}
			else
			{
				char buff[8] 	= {};
				int len 		= 0;
				double num 		= 0;

				int res_of_scan = sscanf((char*)text, "%lf%n", &num, &len);
				
				if(res_of_scan == 1)
				{
					tmp->data.num 	= num;
					tmp->type 		= CONSTANT;

					text += len;
				}
				else
				{
					res_of_scan = sscanf((char*)text, "%[^()]%n", buff, &len);
					printf("read from [");
					for ( int i = 0; i < len; i ++)
						printf("%X", buff[i]);
					printf("]\n");
					
					if(res_of_scan == 1)
						if(len == 1)
						{
							//printf("VAR OR OP:buff is %s\n", buff);
							if(isalpha(buff[0]))
								tmp->type = VARIABLE;
							else
								tmp->type = OPERATOR;
							
							memcpy(tmp->data.str, buff, 8);

							text += len; 
						}
						else 
						{
							//printf("FUNC:buff is %s\n", buff);
							tmp->type = FUNCTION;
							memcpy(tmp->data.str, buff, 8);

							text += len;

						}
					else
						text += len; 
				}

			}
	}
	return res;
}



void PrintTree(FILE* stream, Node* head)
{
	assert(stream);
	assert(head);

	fprintf(stream, "(");

	if(head->left != NULL)
		PrintTree(stream, head->left);

	switch(head->type)
	{
		case CONSTANT:
			fprintf(stream, "%.4lf", head->data.num);
			break;
		default:
			fprintf(stream, "%s", head->data.str);
			break;
	}

	if(head->right != NULL)
		PrintTree(stream, head->right);

	fprintf(stream, ")");
}
*/
//#include "Tree.h"

#include <wchar.h>
#include <locale.h>
#include "./lib/logger/logger.h"
#include "./lib/TextLib/Text.h"
#include "./lib/TextLib/Strings.h"
#include "./lib/TextLib/File.h"
int main(int argc, char* argv [])
{
    printf("current locale(LC_ALL) is %s\n", setlocale(LC_ALL, NULL));
    printf("current locale(LC_CYTPE) is %s\n", setlocale(LC_CTYPE, NULL));
    
    setlocale(LC_ALL, "");
    wchar_t* str = L"Привет Мир! Ты че шутишь блять ебанушка?\n";

    //printf("Mode(stdout) = %d\n", fwide(stdout, 0));

    printText(text_init(argv[1]));

    
    if(fwide(stdout, 1) > 0)
        wprintf(L"Wide string: %ls", str);
    else
        printf("Error ocurred\n");
    
	// if(argc == 1)
	// {
	// 	printf("ERROR: No input file\n");
	// 	return 1;
	// }

	// const char* INPUT = argv[1];

	// size_t size = fsize(INPUT);
	// unsigned char* text = TextFromFile(INPUT, size);


	// Node* head = ReadTreeFrom(text);

	// PrintTree(stdout, head);

	// Node* root = NODE(strdup("animal"));
	// root->left = NODE(strdup("bird"));
	// root->right = NODE(strdup("human"));
	// root->left->right = NODE(strdup("whale"));

	// printTree(root);
	// deleteTree(root);



	return 0;
}
