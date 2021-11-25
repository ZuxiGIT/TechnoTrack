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
#include <wchar.h>
#include <locale.h>
#include "./lib/logger/logger.h"
#include "./lib/TextLib/Text.h"
#include "./lib/TextLib/Strings.h"
#include "./lib/TextLib/File.h"
#include "./lib/tree/tree.h"

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
        
        wscanf(L"%5ls", answer);

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
                
                wprintf(L"Акинатор: Я угадал! Готовь сраку, я выехал :)");

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
                
                wscanf(L"%63ls[^\n]", right_answer);
                //fgetws(right_answer, 64, stdin);


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

                    wscanf(L"%63ls[^ \n]", difference);
                    //fgetws(difference, 64, stdin);

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

                wmemset(right_answer, L'\0', 64);
                wmemset(difference, L'\0', 64);

                current_node = tree->root;
            }
            else
            {
                if(wcscmp(answer, L"exit") == 0)
                    return;

                wmemset(answer, L'\0', 5);
                wprintf(L"Для ответа наберите: [да/нет] ");
                fwscanf(stdin, L"%ls", answer);
            }
    }
}

int main(int argc, char* argv [])
{
    setlocale(LC_ALL, "");
    //printf("current locale(LC_ALL) is %s\n", setlocale(LC_ALL, NULL));
    //printf("current locale(LC_CYTPE) is %s\n", setlocale(LC_CTYPE, NULL));
    

    //printf("Mode(stdout) = %d\n", fwide(stdout, 0));

    //Text* out = text_init(argv[1]);
    //printText(out);
    //text_free(out);
    
    Tree* tree =  tree_init(NULL);
    wprintf(L"++++++++%hhu+++++++\n", (unsigned char)((long)tree&0xff));

    //add_node(tree, tree->root, left, "левый");
    //tree->nodes->left = create_node("левый");
    //tree->nodes->right = create_node("правый");
    //add_node(tree, tree->root, right, "правый");

    playGame(tree);

    save_tree("saveFile.tr", tree);
    dump_tree_dot("out", tree);
    
    tree_free(tree);

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
