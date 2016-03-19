/*
COMPILER PROJECT-2015
Batch Number: 11
2011B2A7744P Sanjeed Schamnad
2011A7PS479P	Shiva Manne
*/

#include<stdlib.h>
#if ! defined _ldef_H
#define _ldef_H 1

#include"lexerDef.h"

#endif

#define NO_OF_TERMINALS 56
#define NO_OF_NON_TERMINALS 51
#define NO_OF_GRAMMAR_RULES 90

#define SYMBOL_TABLE_SIZE1 50
#define MAX_SCOPES 50
#define MAX_CLASH 10
typedef struct node 
{
	int data;
	struct node* link;
} STACK_NODE; 

typedef STACK_NODE* STACK_ELEM;
typedef STACK_NODE* STACK;

typedef	struct
{
	nonTerminal left;
	int right_size;
	int * right;
} Rule;

typedef	struct
{
	terminal* terminal_ptr;
	int size;
} SET;

typedef struct tree_node
{
	int element;
	tokenInfo termToken;
	int ruleUsed;
	int listSize;
	int type;
	int scope_no;
	struct tree_node* children;
	struct tree_node* parent;
	struct tree_node* sibling;
	struct tree_node* lsibling;
} treeNode;

typedef treeNode* parseTree;
typedef treeNode* treeNode_element;

typedef struct
{
	tokenInfo tokenData;
	struct symbol_table_entry* next_token;
	char recordvalue[MAXSTRLEN];
	int type;
	int offset;
} symbol_table_entry;

