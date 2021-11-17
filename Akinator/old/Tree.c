#include "Tree.h"
#include "File.h"
#include <malloc.h>

void _shift(int shift);
Node* _parseNodeFromtext();

Node* _parseNodeFromtext(unsigned char* text)
{
	if (*text == "(")
		text++;
	
	Node* res = createNode(strdup())
	return res;
}


Node* readTreeFromFile(const char* path)
{
	Text* text = text_init(path);

	Node* root = (Node*)calloc(1, sizeof(Node));
	
	for(int i = 0; i < text->num_of_lines; i++)
	{

	}
}

Node* createNode(char* str, Node* lft, Node* rght)
{
    Node* res 	= (Node*)calloc(1, sizeof(Node));
	
    res->data 	= str;
	res->left 	= lft;
	res->right 	= rght;

	return res;
}


void deleteTree(Node* root)
{
	if (root->left)
		deleteTree(root->left);
	if (root->right)
		deleteTree(root->right);
		
	free(root->data);
	free(root);
	return;
}

void printTree(const Node* node)
{
	static int shift = 0;
	_shift(shift);

	printf("(");
	printf("%s", node->data);
	
	shift++;
	
	if(node->left)
	{
		printf("\n");
		printTree(node->left);
	}
	
	if(node->right)
	{
		if (!node->left)
			printf("\n");
	
		printTree(node->right);
	}
	
	shift--;
	
	if(node->left || node->right)
		_shift(shift);
	
	printf(")\n");
}


void _shift(int shift)
{
	for(int i = 0; i < shift; i ++)
		printf("\t");
}