/*
COMPILER PROJECT-2015
Batch Number: 11
2011B2A7744P Sanjeed Schamnad
2011A7PS479P	Shiva Manne
*/

#include<stdio.h>
#if ! defined _l_H
#define _l_H 1

#include"lexer.h"

#endif

#if ! defined _p_H
#define _p_H 1

#include"parser.h"

#endif

char* terminalString1[]={"EPSILON","TK_ASSIGNOP","TK_COMMENT","TK_FIELDID","TK_ID","TK_NUM","TK_RNUM","TK_FUNID","TK_RECORDID","TK_WITH","TK_PARAMETERS","TK_END","TK_WHILE","TK_INT","TK_REAL","TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST",
"TK_SQL","TK_SQR","TK_INPUT","TK_OUTPUT","TK_SEM","TK_COLON","TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_IF","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_PLUS","TK_MINUS","TK_MUL","TK_DIV","TK_CALL","TK_RECORD",
"TK_ENDRECORD","TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE","TK_NE","TK_COMMA","$","TK_ERROR"};




int main(int argc, char* argv[])
{
	int ch=-1;
	char * testFilename = "testcase1.txt";;
	char * parseTreeOutFilename = "parsetreeOutFile.txt";
	char * ASTTreeOutFilename = "astOutFile.txt";
	tokenInfo token;
	int ret;

	FILE * dfaFile=fopen("dfa.txt","r");
	populate_DFA_table(dfaFile);
	
	if(argc>1)
	{
		testFilename = argv[1];
		if(argc>2)
		{
			parseTreeOutFilename = argv[2];
			if(argc>3)
			{
				ASTTreeOutFilename = argv[3];
			}
		}
	}


	FILE* fp=fopen(testFilename,"r");
	removeComments(testFilename);
	FILE *noComments=fopen("noComments.txt","r");

	initialize_first_follow_sets();
	populate_parse_table();

	makeASTtable();
	
	parseTree Parse_tree=initParseTree();
	parseTree AST_tree=initParseTree();
	
	printf("ENTER YOUR CHOICE\n");
	printf("1. Print token list from Lexer.\n");
	printf("2. Parse to verify Syntactic correctness.\n");
	printf("3. Create and print AST into file\n");

	scanf("%d", &ch);
	AST_tree=(parseTree)malloc(sizeof(struct tree_node));
	switch(ch)
	{
		case 1:	
				do
				{
					token=getStream(noComments);
					printf("%-10s\t%-20s\n",terminalString1[token.name], token.value);
				}
				while(token.name!=$ && token.index!=-9);
				break;
				
		case 2: ret=parse_this(noComments, &Parse_tree);
				if(ret>=0)
				{
					printParseTree(Parse_tree, parseTreeOutFilename);
				}
				break;

		case 3: 
				ret=parse_this(noComments, &Parse_tree);
				if(ret>=0)
					printParseTree(Parse_tree, parseTreeOutFilename);
				else
				{
					FILE* f1=fopen(parseTreeOutFilename, "w");
					fprintf(f1, "PARSE TREE NOT PRINTED BECAUSE OF ERRORS IN COMPILATION\n");
					fclose(f1);
				}
				printf("PARSE TREE PRINTED\n\nCOPYING NOW......\n\n");
				
				copyParseTree(Parse_tree, AST_tree);
				
				printf("----------------------Making AST now-------------------------\n\n");
				makeAST(&AST_tree);
				
				//while(AST_tree->parent!=NULL)AST_tree=AST_tree->parent;
				
				if(ret>=0)
					printParseTree(AST_tree, ASTTreeOutFilename);
				else
				{
					FILE* f1=fopen(ASTTreeOutFilename, "w");
					fprintf(f1, "AST NOT PRINTED BECAUSE OF ERRORS IN COMPILATION\n");
					fclose(f1);
				}
				
				makeSymbolTableRoot(AST_tree);
				//printTree(AST_tree);
				print_symbol_table();
				typeCheck(AST_tree);
				printf("GOING FOR SEMANTIC CHECK NOW....\n");
				semanticCheck(AST_tree);
				break;
				
		default:printf("Invalid Choice\n");
				break;
	}

	
	return 0;
}
