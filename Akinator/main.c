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

#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "./lib/logger/logger.h"
#include "./lib/TextLib/Text.h"
#include "./lib/TextLib/Strings.h"
#include "./lib/TextLib/File.h"
#include "./lib/tree/tree.h"

#define SIZE_ARR(arr) sizeof(arr) / sizeof(arr[0])

static inline void wccleanInputBuffer()
{
    wint_t chr = L'0';
    while(((chr = getwc(stdin)) != L'\n') && !feof(stdin) && chr != L'\0') ; //fprintf(stderr, "---> chr %lc\n", chr);
}

static inline void cleanInputBuffer()
{
    char chr = '0';
    while(((chr = fgetc(stdin)) != '\n') && !feof(stdin) && chr != '\0') ; //fprintf(stderr, "---> chr %lc\n", chr);
}

static inline void wcreadInput(wchar_t* input, int size)
{

    
    freopen(NULL, "r", stdin);
    fgetws(input, size, stdin);

    fprintf(stderr, "input: [%ls]\n", input);

    wchar_t* chr = input;

    //deleting last '\n'

    while(*(chr + 1) != L'\0') chr++;
    
    if(*chr == L'\n')
    {
        fprintf(stderr, "got it!\n");
        *chr = L'\0';
    }

    //fprintf(stderr, "input after proccesing: [%ls]\n", input);

    //fprintf(stderr, "before\n");
    fprintf(stderr,"length of input is %lu\n", wcslen(input));

    if(wcslen(input) == (size - 1))
        wccleanInputBuffer();

    //fprintf(stderr, "after\n");
    
    
    freopen(NULL, "r", stdin);
}

static inline void readInput(char* input, int size)
{

    
    freopen(NULL, "r", stdin);
    fgets(input, size, stdin);

    fprintf(stderr, "input(%lu): [%s]\n", strlen(input),  input);

    char* chr = input;

    //deleting last '\n'

    while(*(chr + 1) != '\0') chr++;
    
    if(*chr == '\n')
    {
        fprintf(stderr, "got it!\n");
        *chr = '\0';
    }

    //fprintf(stderr, "input after proccesing: [%ls]\n", input);

    //fprintf(stderr, "before\n");
    fprintf(stderr,"length of input is %lu\n", strlen(input));

    if(strlen(input) == (size - 1))
        cleanInputBuffer();

    //fprintf(stderr, "after\n");
    
    freopen(NULL, "r", stdin);
}


static void playGame(Tree* tree)
{
    wchar_t answer[5] = {};
    wchar_t right_answer[64] = {};
    wchar_t difference[64] = {};

    bool got_answer = false;
    bool end = false;

    Node* current_node = tree->root;

    freopen(NULL, "w", stdout);

    while(!end)
    {
        wprintf(L"Акинатор: Это %ls?\nДля ответа наберите: [да/нет] ",
                current_node->str);
        
        //wscanf(L"%5ls", answer);
        wcreadInput(answer, SIZE_ARR(answer)); //--> just for testing input process
        //fgetws(answer, 5, stdin);

        got_answer = false;

        while(!got_answer)
            if(wcscasecmp(answer, L"да") == 0)
            {
                got_answer = true;

                if(current_node->left != NULL)
                {
                    fprintf(stderr, "--------> Go to left\n");
                    current_node = current_node->left;
                    break;
                }
                
                wprintf(L"Акинатор: Я угадал! Готовь сраку, я выехал :)\n");

                end = true;

                break;
            }
            else if(wcscasecmp(answer, L"нет") == 0)
            {
                got_answer = true;

                if(current_node->right != NULL)
                {
                    fprintf(stderr, "--------> Go to right\n");
                    current_node = current_node->right;
                    break;
                }

                wprintf(L"Акинатор: А что же тогда??\n");
                wprintf(L"Введите имя этой сущности [макс 63 символа]: ");
                
                //wscanf(L"%63ls[^\n]", right_answer);
                //fgetws(right_answer, 64, stdin);
                wcreadInput(right_answer, SIZE_ARR(right_answer));

                /*
                fprintf(stderr, "curr_node->parent->left = %p,
                                curr_node->parent->right = %p,
                                curr_node = %p\n",
                                current_node->parent->left,
                                current_node->parent->right,
                                current_node);
                */

                if (current_node->left != NULL)
                {
                    wadd_node(tree, current_node, right, right_answer);
                }
                else
                {
                    wprintf(L"Акинатор: А чем же тогда они отличаются (%ls и %ls)\n",
                            current_node->str, right_answer);
                    wprintf(L"Введите различие этих сущностей [макс 63 символа]: ");

                    //wscanf(L"%63ls[^ \n]", difference);
                    //fgetws(difference, 64, stdin);
                    wcreadInput(difference, sizeof(difference));

                    if(current_node->parent == NULL)
                    {
                        wadd_node(tree, current_node, right, difference);
                        wadd_node(tree, current_node->right, left, right_answer);
                    }
                    else if(current_node->parent->left == current_node)
                    {
                        //dump_tree_dot("BeforeChangeTree", tree);
                        wadd_node(tree, current_node->parent, left, difference);
                        current_node->parent->left->right = current_node;
                        current_node->parent = current_node->parent->left;
                        wadd_node(tree, current_node->parent, left, right_answer);
                        //dump_tree_dot("AfterChangeTree", tree);
                    }
                    else if(current_node->parent->right == current_node)
                    {
                        //dump_tree_dot("BeforeChangeTree", tree);
                        wadd_node(tree, current_node->parent, right, difference);
                        current_node->parent->right->right = current_node;
                        current_node->parent = current_node->parent->right;
                        wadd_node(tree, current_node->parent, left, right_answer);
                        //dump_tree_dot("AfterChangeTree", tree);
                    }
                    else
                    {
                        pr_err(LOG_CONSOLE, "ERROR: Bad tree\n");
                        dump_tree_dot("BadTreeDump", tree);
                        exit(1);
                    }
                }

                wmemset(right_answer, L'\0', SIZE_ARR(right_answer));
                wmemset(difference, L'\0', SIZE_ARR(difference));

                current_node = tree->root;
            }
            else
            {
                if(wcscmp(answer, L"exit") == 0)
                    return;

                wmemset(answer, L'\0', 5);
                wprintf(L"Для ответа наберите: [да/нет] ");

                //fwscanf(stdin, L"%5ls", answer);
                //fgetws(answer, 5, stdin);
                wcreadInput(answer, SIZE_ARR(answer));
            }
    }
}

typedef long long ll;
#define StkElem ll
#include "./lib/Stack/Stack.h"

static void getDefinition(wchar_t* str, Tree* tree)
{
    STACK(ll) stk = {};
    CTOR(ll, stk,  tree->size);
   
}

static inline void printUsage()
{
    wprintf(L"1) Играть\n"
            L"2) Сохранить игру\n"
            L"3) Дамп дерева\n"
            L"4) Выход\n"
            L"");
}

static inline void saveTree(Tree* tree)
{
    char output[64] = {};

    wprintf(L"Куда сохраняем? (use eng): ");
    fflush(stdout);
    readInput(output, SIZE_ARR(output));

    save_tree(output, tree);
}

static inline void dumpTree(Tree* tree)
{
    char output[64] = {};

    wprintf(L"Куда дампим? (use eng): ");
    fflush(stdout);
    readInput(output, SIZE_ARR(output));

    dump_tree_dot(output, tree);
}

static void mainMenu()
{
    bool end = false;

    Tree* tree =  tree_init(NULL);

    int res = 0;

    while(!end)
    {
        printUsage();

        wprintf(L"Выберите пункт: ");

        while(wscanf(L"%d", &res) != 1)
        {
            wccleanInputBuffer();
            wprintf(L"Выберите пункт: ");
        }

        wccleanInputBuffer();
        //while(scanf("%d", &res) != 1) ;

        // TODO
        // do enum
        switch(res)
        {
            case 1:
                playGame(tree);
                break;
            case 2:
                saveTree(tree);
                break;
            case 3:
                dumpTree(tree);
                break;
            case 4:
            default:
                end = true;
        }
    }
    
    tree_free(&tree);
}

int main(int argc, char* argv [])
{
    setlocale(LC_ALL, "");
    //printf("current locale(LC_ALL) is %s\n", setlocale(LC_ALL, NULL));
    //printf("current locale(LC_CYTPE) is %s\n", setlocale(LC_CTYPE, NULL));
    
    mainMenu();

	return 0;
}
