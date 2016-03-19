/*
COMPILER PROJECT-2015
Batch Number: 11
2011B2A7744P Sanjeed Schamnad
2011A7PS479P	Shiva Manne
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#if ! defined _p_H
#define _p_H 1

#include"parser.h"

#endif
#include"lexer.h"

//#define DEBUG_PARSE
//#define DEBUG_PARSE_PRINT
//#define DEBUG_AST
//#define DEBUG_SYMBOL_TABLE

int COUNT_EPSILON=0;
int COUNT_ELEM=0;


Rule GRAMMAR_RULES[NO_OF_GRAMMAR_RULES];
SET FIRST[NO_OF_NON_TERMINALS];
SET FOLLOW[NO_OF_NON_TERMINALS];


int NOF_GRAMMAR_RULES=0;
int NOF_FIRST_SET_ELEMENTS=0;
int NOF_FOLLOW_SET_ELEMENTS=0;

symbol_table_entry* Symbol_table[MAX_SCOPES][SYMBOL_TABLE_SIZE1];
int SYMBOL_TABLE_SIZE=0;				//used for token index
int NO_OF_SCOPES=1;

int nofscopes=1;

char recordval[MAXSTRLEN];
char funname[MAXSTRLEN];

int ast_rules[NO_OF_GRAMMAR_RULES][10];

char* terminalString[]={"EPSILON","TK_ASSIGNOP","TK_COMMENT","TK_FIELDID","TK_ID","TK_NUM","TK_RNUM","TK_FUNID","TK_RECORDID","TK_WITH","TK_PARAMETERS","TK_END","TK_WHILE","TK_INT","TK_REAL","TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST",
"TK_SQL","TK_SQR","TK_INPUT","TK_OUTPUT","TK_SEM","TK_COLON","TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_IF","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_PLUS","TK_MINUS","TK_MUL","TK_DIV","TK_CALL","TK_RECORD",
"TK_ENDRECORD","TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE","TK_NE","TK_COMMA","$","TK_ERROR"};


char* nonterminalString[]=
{
"<program>","<otherFunctions>","<mainFunction>","<stmts>","<function>","<input_par>","<output_par>","<parameter_list>","<dataType>","<remaining_list>","<primitiveDatatype>","<constructedDatatype>","<typeDefinitions>",
"<declarations>","<returnStmt>","<typeDefinition>","<fieldDefinitions>","<fieldDefinition>","<moreFields>","<declaration>","<global_or_not>","<stmt>","<otherStmts>","<assignmentStmt>","<iterativeStmt>","<conditionalStmt>",
"<ioStmt>","<funCallStmt>","<SingleOrRecId>","<arithmeticExpression>","<outputParameters>","<inputParameters>","<idList>","<booleanExpression>","<elsePart>","<allVar>","<var>","<term>","<expPrime>","<lowPrecenceOperators>",
"<factor>","<termPrime>","<highPrecedenceOperators>","<all>","<temp>","<operator>","<logicalOp>","<relationalOp>","<optionalReturn>","<more_ids>","<singleOrRecIdPrime>"
};

typedef enum {INVALID=-1, INT, REAL, RECORD, BOOL} TYPE;

int PARSE_TABLE[NO_OF_NON_TERMINALS][NO_OF_TERMINALS];

extern STACK initStack(void)
{
	return (STACK)NULL;
}

extern STACK_ELEM getStackNode()
{
	STACK_ELEM tmp;
	tmp=(STACK_ELEM) malloc (sizeof (STACK_NODE));
	tmp->link=NULL;
	return tmp;
}


extern void push (STACK* Top, int a)
{
	STACK_ELEM tmp=getStackNode();
	int i;

	tmp->data=a;

	if (*Top==NULL)
	{
		*Top=tmp;
		(*Top)->link=NULL;
	}
	else
	{
		tmp->link=*Top;		// push the data
		*Top=tmp;
	}
}

extern BOOLEAN pop (STACK* Top, int* a)
{
	int i;
	STACK_ELEM tmp=*Top;
	if (!empty(tmp))
	{
		*a=tmp->data;
		*Top=(*Top)->link;
		free (tmp);
		return true;
	}
	else
		return false;

}


extern BOOLEAN top (STACK* Top, int* a)
{
	int i;
	if (*Top!=NULL)
	{
		*a=((*Top)->data);
		return true;
	}
	else false;
}


extern BOOLEAN empty(STACK Top)
{
	if (Top==NULL)
		return true;
	else
		return false;
}

extern void display(STACK Top)
{
	int i;
	STACK_ELEM tmp=Top;
	int c=count(Top);
	printf("\n The number of elements int the stack : %d ",c);
	if (!empty(Top))
	{
		printf("\n Stack Contents: \n Top -->  \t");
		while (!empty(tmp))
		{
			printf(" [ ");
			printf(" %d ", tmp->data);
			printf("] -> \t");
			tmp=tmp->link;
		}
		printf("\n\n -> \t");
	}
	else printf("\n Empy Stack \n");
	printf("\n");

}

extern int count(STACK Top)
{
	int i, c=0;
	STACK_ELEM tmp=Top;
	if (empty(tmp)==false)
	{
		while (tmp!=NULL)
		{
			++c;
			tmp=tmp->link;
		}
	}
	return c;
}


int getListSize(parseTree node)
{
	int count=0;
	if(node!=NULL)
	{
		node=node->children;
		while(node!=NULL)
		{
			count++;
			node=node->sibling;
		}
		return count;
	}
	else return count;
}


parseTree initParseTree()
{
	return (parseTree)NULL;
}

treeNode_element getNewNode()
{
	treeNode_element temp;
	temp = (treeNode_element)malloc(sizeof(struct tree_node));
	temp->listSize=0;
	temp->termToken.index=-1;
	temp->ruleUsed=-1;
	temp->type=-1;
	temp->scope_no=-1;
	temp->parent=NULL;
	temp->children=NULL;
	temp->sibling=NULL;
	temp->lsibling=NULL;
	return temp;
}

int insertInto(treeNode_element* P, int a)
{
	int count=0;
	treeNode_element new_node=getNewNode();
	new_node->element= a;
	new_node->listSize=0;

	if((*P)==NULL)
		(*P)=new_node;
	else
	{
		treeNode_element temp, temp1;
		temp1=(*P);
		temp= (treeNode_element) (*P)->children;
		(*P)->listSize++;
		if(temp==NULL)
		{
			(*P)->children=new_node;
			new_node->parent=(*P);
		}
		else
		{
			while(temp->sibling!=NULL)
			{
				temp->parent=(*P);
				temp = (treeNode_element)temp->sibling;

				count++;
			}
			new_node->parent = (*P);
			temp->sibling = new_node;
			new_node->lsibling = temp;
		}
	}
}


parseTree copyParseTree(parseTree fromPT, parseTree toPT)
{
	if(fromPT!=NULL)
	{
		if(toPT==NULL)
		{
			toPT=(parseTree)malloc(sizeof(struct tree_node));
			toPT->element= fromPT->element;
			toPT->listSize=fromPT->listSize;
			toPT->type=fromPT->type;
			toPT->scope_no=fromPT->scope_no;
			toPT->parent = fromPT->parent;
			toPT->children = fromPT->children;
			toPT->sibling = fromPT->sibling;
			toPT->lsibling = fromPT->lsibling;
			toPT->termToken.index=-1;
			//insertInto(&toPT, fromPT->element);
		}
		else
		{
			toPT->termToken.index=-1;
			toPT->element= fromPT->element;
			toPT->listSize=fromPT->listSize;
			toPT->type=fromPT->type;
			toPT->scope_no=fromPT->scope_no;
			toPT->parent = fromPT->parent;
			toPT->children = fromPT->children;
			toPT->sibling = fromPT->sibling;
			toPT->lsibling = fromPT->lsibling;
		}
		fromPT=fromPT->children;
		toPT=toPT->children;
		while(fromPT!=NULL)
		{
			toPT=copyParseTree(fromPT, toPT);
			fromPT=fromPT->sibling;
			if(toPT!=NULL && toPT->sibling!=NULL)
			toPT=toPT->sibling;
		}
	}
	return toPT;
}


void printTree(parseTree  PT)	//PRINTING IN Depth First Order
{
	if(PT!=NULL)
	{

		printf("ELEM:%d\tVALUE:%s\tTYPE:%d\tSCOPE:%d\n", (PT)->element, PT->termToken.value, PT->type, PT->scope_no);
		int count;
		treeNode_element temp=(treeNode_element) PT;
		count=temp->listSize;
		if(temp->children!=NULL)
		{
			temp=temp->children;

			while(1)
			{
				printTree(temp);
				if(temp->sibling!=NULL)
					temp=temp->sibling;
				else
					break;
			}

		}
	}
}



int printParseTree(parseTree  PT, char* outfile)	//PRINTING IN Depth First Order
{
	FILE * fp=fopen(outfile, "w");
	fprintf(fp, "lexemeCurrentNode\tlineno\ttoken\t\t\tvalueIfNumber\t\tparentNodeSymbol\tisLeafNode(yes/no)\tNodeSymbol\n");
	#ifdef DEBUG_PARSE
	printf("lexemeCurrentNode\tlineno\ttoken\t\t\tvalueIfNumber\t\tparentNodeSymbol\tisLeafNode(yes/no)\tNodeSymbol\n");
	#endif
	COUNT_EPSILON=0;
	COUNT_ELEM=0;
	printParseTreeRecur(PT,fp);
//	printf("COUNTEPSILON:%d  COUNTELEM:%d\n", COUNT_EPSILON, COUNT_ELEM);
	fclose(fp);
	return COUNT_ELEM*sizeof(struct tree_node);
}


void printParseTreeRecur(parseTree  PT, FILE *fp)	//PRINTING IN Depth First Order
{
	if(PT!=NULL)
	{

		if(PT->element<NO_OF_TERMINALS)
		{
			fprintf(fp, "%-20s\t%d\t%-20s\t",  PT->termToken.value, PT->termToken.lineNumber, terminalString[PT->termToken.name] );
			if(PT->element==TK_NUM||PT->element==TK_RNUM)
			{
				fprintf(fp, "%-10s\t", PT->termToken.value);
			}
			else
			{
				fprintf(fp, "----      \t");
			}
			if(PT->parent!=NULL)
				fprintf(fp, "%-30s\t", nonterminalString[PT->parent->element-NO_OF_TERMINALS]);
			else
				fprintf(fp,"ROOT                          \t");
			fprintf(fp, "Yes\t\t----                          \t");

		}
		else
		{
			fprintf(fp, "----                \t--\t----                \t----      \t");
			if(PT->parent!=NULL)
				fprintf(fp, "%-30s\t", nonterminalString[PT->parent->element-NO_OF_TERMINALS]);
			else
				fprintf(fp, "ROOT                          \t");
			fprintf(fp, "No \t\t%-30s\t",nonterminalString[PT->element-NO_OF_TERMINALS]);


		}
		fprintf(fp, "\n");



		#ifdef DEBUG_PARSE_PRINT
//		printf("%d\t%d\t%d\n", PT->element, PT->listSize, PT->ruleUsed);
		if(PT->element==0)COUNT_EPSILON++;
		COUNT_ELEM++;
		if(PT->element<NO_OF_TERMINALS)
		{
			printf("%-20s\t%d\t%-20s\t",  PT->termToken.value, PT->termToken.lineNumber, terminalString[PT->termToken.name] );
			if(PT->element==TK_NUM||PT->element==TK_RNUM)
			{
				printf("%-10s\t\t", PT->termToken.value);
			}
			else
			{
				printf("----      \t\t");
			}
			if(PT->parent!=NULL)
				printf("%-30s\t", nonterminalString[PT->parent->element-NO_OF_TERMINALS]);
			else
				printf("ROOT                          \t");
			printf("Yes\t\t----                          \t");

		}
		else
		{
			printf("----                \t--\t----                \t----      \t\t");
			if(PT->parent!=NULL)
				printf("%-30s\t", nonterminalString[PT->parent->element-NO_OF_TERMINALS]);
			else
				printf("ROOT                          \t");
			printf("No \t\t%-30s\t",nonterminalString[PT->element-NO_OF_TERMINALS]);


		}
		printf("\n");
		#endif


		int count;
		treeNode_element temp=(treeNode_element) PT;
		count=temp->listSize;
		if(temp->children!=NULL)
		{
			temp=temp->children;
			while(1)
			{
				printParseTreeRecur(temp, fp);
				if(temp->sibling!=NULL)
					temp=temp->sibling;
				else
					break;
			}

		}
	}
}



int initialize_first_follow_sets()
{

	int NT_NUM=0;
	int set_size=0;
	nonTerminal NT_POS=0;


	int temp1=0, temp2=0, temp3=0;
	/////////////////////////////////////////////////FILLING FIRST SETS////////////////////////////////////////////////////////////

	FILE *firstFile=fopen("FIRST.txt", "r");
	fscanf(firstFile, "%d\n", &NT_NUM);
	while(NT_NUM--)
	{
		NOF_FIRST_SET_ELEMENTS++;
		fscanf(firstFile, "%d %d\n", &temp1, &temp2);
		NT_POS=(nonTerminal)temp1;
		set_size=temp2;
		FIRST[NT_POS].size=set_size;
		FIRST[NT_POS].terminal_ptr=(terminal*)malloc(set_size*sizeof(terminal));
		while(temp2)
		{
			fscanf(firstFile, "%d", &temp3);
			*(FIRST[NT_POS].terminal_ptr+(set_size-temp2))=temp3;
			temp2--;
		}
		set_size=temp2=0;

	}


	/////////////////////////////////////////////////FILLING FOLLOW SETS////////////////////////////////////////////////////////////

	FILE *followFile=fopen("FOLLOW.txt", "r");
	fscanf(followFile, "%d\n", &NT_NUM);
	while(NT_NUM--)
	{
		NOF_FOLLOW_SET_ELEMENTS++;
		fscanf(followFile, "%d %d\n", &temp1, &temp2);
		NT_POS=(nonTerminal)temp1;
		set_size=temp2;

		FOLLOW[NT_POS].size=set_size;
		FOLLOW[NT_POS].terminal_ptr=(terminal*)malloc(set_size*sizeof(terminal));
		while(temp2)
		{
			fscanf(followFile, "%d", &temp3);
			*(FOLLOW[NT_POS].terminal_ptr+(set_size-temp2))=temp3;
			temp2--;
		}
		set_size=temp2=0;
	}

	/////////////////////////////////////////////////FILLING GRAMMAR RULES////////////////////////////////////////////////////////////

	FILE *grammar_file=fopen("GRAMMAR.txt", "r");
	fscanf(grammar_file, "%d\n", &NT_NUM);
	char typ='\0';
	while(NT_NUM--)
	{
		fscanf(grammar_file, "%d %d\n", &temp1, &temp2);

		NT_POS=(nonTerminal)temp1;
		GRAMMAR_RULES[NOF_GRAMMAR_RULES].left=NT_POS;
		set_size=temp2;

		GRAMMAR_RULES[NOF_GRAMMAR_RULES].right_size=set_size;
		GRAMMAR_RULES[NOF_GRAMMAR_RULES].right=(int*)malloc(set_size*sizeof(int));


		while(temp2)
		{
			fscanf(grammar_file, "\n%c\n", &typ);
			fscanf(grammar_file, "%d", &temp3);

			if(typ=='N')
				*(GRAMMAR_RULES[NOF_GRAMMAR_RULES].right+(set_size-temp2)) = temp3 + NO_OF_TERMINALS;
			else if(typ=='T')
				*(GRAMMAR_RULES[NOF_GRAMMAR_RULES].right+(set_size-temp2)) = temp3;
			else
			{
				printf("ERROR IN GRAMMAR FILE FORMAT..%c. USE 'T' for TERMINAL and 'N' for NON_TERMINAL\n", typ);
				exit(0);
			}
			temp2--;
		}
		set_size=temp2=0;
		NOF_GRAMMAR_RULES++;
	}

	int ij1=0, ij2=0;
	for(ij1=0; ij1<NO_OF_NON_TERMINALS; ij1++)
		for(ij2=0; ij2<NO_OF_TERMINALS; ij2++)
		{
			PARSE_TABLE[ij1][ij2]=-1;
		}

	int init_i=0, init_j=0;
	for(init_j=0; init_j<MAX_SCOPES; init_j++)
	for(init_i=0; init_i<SYMBOL_TABLE_SIZE1; init_i++)
		Symbol_table[init_j][init_i]=NULL;

}


void populate_parse_table()
{
	int rule_count=0, term_count=0;
	terminal term_i=0;
	int term_size=0, eps_count=0, isEpsilon=0;;
	int element_input=-1;
	nonTerminal NT;

	while(rule_count < NOF_GRAMMAR_RULES)
	{
		NT = GRAMMAR_RULES[rule_count].left;
		isEpsilon=1;
		term_count=0;
		eps_count=0;

		while(isEpsilon==1)
		{
			isEpsilon=0;
			if(GRAMMAR_RULES[rule_count].right_size>0)
			{
				if( eps_count< GRAMMAR_RULES[rule_count].right_size)
				{
					element_input=*(GRAMMAR_RULES[rule_count].right+eps_count);
					if(element_input < NO_OF_TERMINALS &&  element_input>0)
					{
						isEpsilon=0;
						PARSE_TABLE[NT][element_input] = rule_count;
					}
					else if(element_input == 0)
					{
						isEpsilon=1;
						eps_count++;
					}
					else if(element_input>NO_OF_TERMINALS && element_input < NO_OF_TERMINALS+NO_OF_NON_TERMINALS)
					{
						element_input=element_input- NO_OF_TERMINALS;
						term_size = FIRST[element_input].size;

						while(term_count<term_size)
						{

							term_i=*(FIRST[element_input].terminal_ptr+term_count);
							if(term_i!=EPSILON)
							{
								if(PARSE_TABLE[NT][term_i]==-1 || PARSE_TABLE[NT][term_i]==rule_count)
								{
									PARSE_TABLE[NT][term_i] = rule_count;
								}
								else
									printf("ERROR: NOT LL(1) for NT : %d\n", NT);
							}
							else
							{
								isEpsilon=1;
								eps_count++;
							}
							term_count++;
						}
						term_count=0;
					}
				}
				else
				{
					isEpsilon=0;
					term_size=FOLLOW[NT].size;
					while(term_count<term_size)
					{
						term_i=*(FOLLOW[NT].terminal_ptr+term_count);
						if(term_i!=EPSILON)
						{
							if(PARSE_TABLE[NT][term_i]==-1)
							{
								PARSE_TABLE[NT][term_i] = rule_count;
							}
							else
									printf("ERROR: NOT LL(1) for NT : %d\n", NT);
							}
							term_count++;
					}
					term_count=0;
				}
			}
		}
		rule_count++;
	}

}


int parse_this(FILE *instream, parseTree* Parse_tree)
{

	STACK parse_stack=initStack();
	push(&parse_stack, $);
	push(&parse_stack, NO_OF_TERMINALS);

	insertInto(Parse_tree, NO_OF_TERMINALS);

	terminal input_sym;
	int rule_to_use, temp2=0;
	int gen_tp=0;
	int exit_all=0;

	int toexit=0;
	int temp1=0;
	int top_nt;
	terminal check_t;
	nonTerminal new_nt;
	int check_nt=0;
	int right_cnt=0;
	treeNode_element new_node1;
	tokenInfo token_to_parse;

	token_to_parse=getStream(instream);
	input_sym=token_to_parse.name;
	treeNode_element NT_to_parse=*Parse_tree;

	BOOLEAN check;
	int last_tym=0;
	while(last_tym<1)
	{
		top(&parse_stack, &temp1);
		#ifdef DEBUG_PARSE
			printf("TOP OF STACK IS:::::: %d\n", temp1);
		#endif

		top_nt=temp1;

		#ifdef DEBUG_PARSE
			printf("token read is: %d\t", input_sym);
		#endif
		if( rule_to_use==-2 || input_sym==TK_ERROR)
		{
			//Condition when unknown pattern in lexer found -- to continue checking lexer errors
			rule_to_use=-2;
			#ifdef DEBUG_PARSE
				printf("Parsing stopped due to last errors!!!\n");
			#endif
			token_to_parse=getStream(instream);
			input_sym=token_to_parse.name;
			if(input_sym==$)
				last_tym++;

		}
		else
		{
			if(top_nt>=NO_OF_TERMINALS  && top_nt<NO_OF_NON_TERMINALS + NO_OF_TERMINALS)
			{
				new_nt=top_nt-NO_OF_TERMINALS;
				rule_to_use = PARSE_TABLE[new_nt][input_sym];

				#ifdef DEBUG_PARSE
					printf("RULE TO USE::: %d \n", rule_to_use);
				#endif
				if(rule_to_use!=-1 && GRAMMAR_RULES[rule_to_use].left == new_nt)
				{
					check=pop(&parse_stack, &check_nt);
					#ifdef DEBUG_PARSE
						printf("checking non term:::: %d %d\n", check_nt, check);
					#endif
					if(NT_to_parse->children!=NULL)
					NT_to_parse=(treeNode_element)NT_to_parse->children;
					if(check_nt==top_nt)
					{

						right_cnt=GRAMMAR_RULES[rule_to_use].right_size -1;
						while(right_cnt>=0)
						{
							#ifdef DEBUG_PARSE
								printf("pushing:  %d\t",  *(GRAMMAR_RULES[rule_to_use].right+right_cnt));
							#endif
							push(&parse_stack, *(GRAMMAR_RULES[rule_to_use].right+right_cnt));
							#ifdef DEBUG_PARSE
								printf("\ninserting in parse tree: %d for nt: %d\n", *(GRAMMAR_RULES[rule_to_use].right+(GRAMMAR_RULES[rule_to_use].right_size-1-right_cnt)), NT_to_parse->element);
							#endif
							insertInto(&NT_to_parse, *(GRAMMAR_RULES[rule_to_use].right+(GRAMMAR_RULES[rule_to_use].right_size-1-right_cnt)));
							NT_to_parse->ruleUsed=rule_to_use;
							right_cnt--;
						}
					}
					else
					{
						return -1;
					}
				}
				else
				{
					printf("ERROR_5 Cannot Parse. Token %d at line number: %d\n", token_to_parse.name, token_to_parse.lineNumber);
					return -1;
				}
			}
			else
			{
				if(top_nt==0)
				{
					pop(&parse_stack, (int*)&check_t);
					if(NT_to_parse->children!=NULL)
						NT_to_parse=NT_to_parse->children;
					if(NT_to_parse->sibling!=NULL)
						NT_to_parse=NT_to_parse->sibling;
					else
					{
						while(toexit!=2 && NT_to_parse->element!=NO_OF_TERMINALS)
						{
							if(NT_to_parse->parent!=NULL)
							{
								NT_to_parse=NT_to_parse->parent;
								toexit=1;
							}
							if(NT_to_parse->sibling!=NULL)
							{
								NT_to_parse=NT_to_parse->sibling;
								toexit=2;
							}

						}
						toexit=0;
					}
				}
				else if(top_nt==input_sym)
				{
					#ifdef DEBUG_PARSE
						printf("removing token :%d from stack...\n", top_nt);
					#endif
					pop(&parse_stack, (int*)&check_t);
					if(NT_to_parse->children!=NULL)
						NT_to_parse=NT_to_parse->children;
					//-----tokenInfo populate for teminal
					if(NT_to_parse->element==input_sym)
					{
						NT_to_parse->termToken=token_to_parse;
					}
					if(NT_to_parse->sibling!=NULL)
					{
						NT_to_parse=NT_to_parse->sibling;
					}
					else
					{

						while(toexit!=2 && NT_to_parse->element!=NO_OF_TERMINALS)
						{
							if(NT_to_parse->parent!=NULL)
							{
								NT_to_parse=NT_to_parse->parent;
								toexit=1;
							}
							if(NT_to_parse->sibling!=NULL)
							{
								NT_to_parse=NT_to_parse->sibling;
								toexit=2;
							}

						}
						toexit=0;
					}

					top(&parse_stack, &temp1);
					if(temp1!=$)
					{
						token_to_parse=getStream(instream);
						input_sym=token_to_parse.name;
						if(input_sym==$)
							last_tym++;
					}
					else
					{
						token_to_parse=getStream(instream);
						input_sym=token_to_parse.name;
						if(input_sym==$)
						{
							printf("SUCCESSFUL PARSING DONE......\n\n");
							return 1;
						}
						else
						{
							printf("ERROR_4: EXTRA TOKENS LEFT IN THE INPUT AFTER PARSING AT LINE %d......\n\n", token_to_parse.lineNumber);
							return -1;

						}
					}
				}
				else
				{
					printf("TERMINAL NOT MATCHING!!!!\n");
					return -1;
				}
			}
		}
	}
	return rule_to_use;
}

//SYMBOL TABLE FUNCTIONS

unsigned int hash_symbol_table(char *str)
{
	unsigned int hash=81;
	int c;
	while (c = *str++)
	{
		hash = ((hash << 5) + hash) + c;		// hash*33 +c
	}
	hash=hash%50;
	return hash;
}


symbol_table_entry* getNewSymbolTableEntry()
{
	symbol_table_entry* new;
	new=(symbol_table_entry*)malloc(sizeof(symbol_table_entry));
	new->next_token=NULL;
	new->type=-1;
	strcpy(new->recordvalue, "\0");
	new->offset=-1;
}


int insertinSymbolTable(tokenInfo tok, int scope_no, int hashed_ind)
{
	symbol_table_entry* new_entry=getNewSymbolTableEntry();
	new_entry->tokenData=tok;
	int count=0;

	if(Symbol_table[scope_no][hashed_ind]==NULL)
		Symbol_table[scope_no][hashed_ind]=new_entry;
	else
		if(strcmp(Symbol_table[scope_no][hashed_ind]->tokenData.value, tok.value))
		{
			symbol_table_entry* temp=Symbol_table[scope_no][hashed_ind];

			while(temp->next_token!=NULL)
			{
				temp=(symbol_table_entry*)temp->next_token;
				if(!strcmp(temp->tokenData.value, tok.value))
					return -1;
				count++;
			}
			printf("-------------------------------------------SYMBOL HASH TABLE CLASH DETECTED-----------------------------------------\n");
			temp->next_token=(struct symbol_table_entry*)new_entry;
		}
		else
			return -1;
	return count;
}


void print_symbol_table()
{
	printf("NO_OF_SCOPES:%d\n", NO_OF_SCOPES);
	int temp_j=0;
	int cnt=0;
	for(temp_j=0;temp_j<=NO_OF_SCOPES;temp_j++)
	{
		printf("\n\n==================================================SYMBOL TABLE at scope %d===============================================\n", temp_j);
		printf("SNO\tCHAINNO\tINDEX\tLINE NUMBER\tTOKEN\t\tVALUE\tTYPE\t\tOFFSET\tRECORD\n");
		int temp_i=0;

		symbol_table_entry* temp1=NULL;
		for(temp_i=0; temp_i<SYMBOL_TABLE_SIZE1; temp_i++)
		{
			cnt=0;
			temp1=Symbol_table[temp_j][temp_i];
			while(temp1 != NULL)
			{
				printf("%d\t%d\t%d\t%d\t\t%d\t\t\"%s\"\t%d\t\t%d\t%s\n", temp_i, cnt, temp1->tokenData.index, temp1->tokenData.lineNumber, temp1->tokenData.name, temp1->tokenData.value, temp1->type, temp1->offset, temp1->recordvalue );
				temp1=(symbol_table_entry*)temp1->next_token;
				cnt++;
			}
		}
	}
}

int searchIndex(tokenInfo tok, int scope_no, int hashed_index)
{
	symbol_table_entry* temp=Symbol_table[scope_no][hashed_index];
	int count=0;
	while(temp != NULL)
	{
		if(!strcmp(temp->tokenData.value, tok.value))
			return count;
		temp=(symbol_table_entry*)temp->next_token;
		count++;
	}
	return -1;
}


void makeSymbolTable(parseTree PT)
{
	//printf("here\n");
	if(PT!=NULL)
	{
		#ifdef DEBUG_SYMBOL_TABLE
		printf("PT is: %d\n", PT->element);
		#endif
		if(PT->element==TK_FUNID && (PT->parent->element==otherFunctions+NO_OF_TERMINALS||PT->parent->element==program+NO_OF_TERMINALS)  ||(PT->element==TK_MAIN && PT->parent->element==mainFunction+NO_OF_TERMINALS))
		{
			#ifdef DEBUG_SYMBOL_TABLE
			printf("HERE!!!! function changing %s\n", PT->termToken.value);
			#endif
			if(PT->element==TK_FUNID)
			{
				NO_OF_SCOPES++;
				nofscopes++;
			}
			else
			{
				#ifdef DEBUG_SYMBOL_TABLE
				printf("IN MAIN NOW\n");
				#endif
				nofscopes=1;
			}
		}

		if(PT!=NULL && PT->element==TK_RECORDID && PT->parent->element==typeDefinitions+NO_OF_TERMINALS)
		{
			strcpy(recordval, PT->termToken.value);
			#ifdef DEBUG_SYMBOL_TABLE
			printf("New Record id: %s\n", recordval);
			#endif
		}
		if(PT!=NULL && PT->termToken.name==TK_FIELDID)
		{
			unsigned int hash_index=0;
			int ret=0;
			hash_index=hash_symbol_table(PT->termToken.value);
			#ifdef DEBUG_SYMBOL_TABLE
			printf("Hash index is: %d for string: %s at scope: %d\n", hash_index, PT->termToken.value, nofscopes);
			#endif
			if(PT->parent->element==fieldDefinition+NO_OF_TERMINALS|| PT->parent->element==parameter_list+NO_OF_TERMINALS)
			{
/*				ret=insertinSymbolTable(PT->termToken, nofscopes, hash_index);
				if(ret<0)
				{

					//ERROR REDECLARATION OF VARIABLE
					printf("-----------------------error redeclaration of variable ----------\n");
				}
				else	PT->termToken.index=hash_index*MAX_CLASH + ret;
				int type=-1;
				if(PT->lsibling!=NULL)
				{
					if(PT->lsibling->element==TK_INT)
						type=INT;
					else if(PT->lsibling->element==TK_REAL)
						type=REAL;
					else if(PT->lsibling->element==TK_RECORDID)
						type=RECORD;
					symbol_table_entry * temp= Symbol_table[nofscopes][hash_index];
//					printf("TEMP %d\n",(temp==NULL)?0:1);
					while(temp!=NULL && strcmp(temp->tokenData.value, PT->termToken.value)!=0)
					{
						temp=(symbol_table_entry*)temp->next_token;
					}
					if(temp!=NULL)
					{	temp->type=type;
						strcpy(temp->recordvalue, recordval);
					}
				}
*/
			}
			else
			{
				ret=searchIndex(PT->termToken, 0, hash_index);
				if(ret<0)
				{
/*					//ERROR NOT DECLARED*/
/*					int gen_i=0, ret1=-1, gen_j=0, gen_k=0, ret2=-1;*/
/*					for(gen_i=0;gen_i<NO_OF_SCOPES;gen_i++)*/
/*					{*/
/*						ret1=searchIndex(PT->termToken, gen_i, hash_index);*/
/*						if(ret1>=0)*/
/*						{*/
/*							symbol_table_entry * temp= Symbol_table[gen_i][hash_index];*/
/*			//				#ifdef DEBUG_SYMBOL_TABLE*/
/*							printf("found at %d scope\n", gen_i);*/
/*			//				#endif*/
/*							while(gen_j<ret1)*/
/*							{*/
/*								temp=(symbol_table_entry*)temp->next_token;*/
/*								gen_j++;*/
/*							}*/
/*							*/
/*							ret2=insertinSymbolTable(PT->termToken, nofscopes, hash_index);*/
/*							symbol_table_entry * temp1= Symbol_table[nofscopes][hash_index];*/
/*							PT->termToken.index=hash_index*MAX_CLASH + ret2; */
/*							while(gen_k<ret2)*/
/*							{*/
/*								temp1=(symbol_table_entry*)temp1->next_token;*/
/*								gen_k++;*/
/*							}*/
/*							//temp1->scope=nofscopes;*/
/*							temp1->type=temp->type;*/
/*							strcpy(temp1->recordvalue, temp->recordvalue);*/
/*							temp1->offset=temp->offset;*/
/*							*/
/*							break;*/
/*						}*/
/*					}*/
/*					if(ret1<0)*/
					printf("-----------------------error not declared variable1 ----------\n");
				}
				else
				{
					PT->termToken.index=hash_index*MAX_CLASH + ret;
					PT->scope_no=0;
					symbol_table_entry * temp= Symbol_table[0][hash_index];
					int gen_j=0;
					while(gen_j<ret)
					{
						temp=(symbol_table_entry*)temp->next_token;
						gen_j++;
					}
					PT->type=temp->type;
				}
			}
		}

		if(PT!=NULL && PT->termToken.name==TK_ID && (PT->sibling==NULL || PT->sibling->element!=TK_GLOBAL))
		{

			unsigned int hash_index=0;
			int ret=0;
			hash_index=hash_symbol_table(PT->termToken.value);
			#ifdef DEBUG_SYMBOL_TABLE
			printf("Hash index is: %d for string: %s at scope: %d\n", hash_index, PT->termToken.value, nofscopes);
			#endif
			if(PT->parent->element==parameter_list+NO_OF_TERMINALS || PT->parent->element==declaration+NO_OF_TERMINALS)
			{
				int ret1=searchIndex(PT->termToken, 0, hash_index);
				if(ret1>=0)
				{
					printf("ERROR: Redeclaration of variable %s. Earlier declaration was global.\n", PT->termToken.value);
				}
				else
				{
					ret=insertinSymbolTable(PT->termToken, nofscopes, hash_index);

					if(ret<0)
					{
						//ERROR REDECLARATION OF VARIABLE
						printf("ERROR: Redeclaration of variable %s in this scope.\n", PT->termToken.value);
					}
					else
					{
						PT->termToken.index=hash_index*MAX_CLASH + ret;
						PT->scope_no=nofscopes;
					}

					int type=-1;
					if(PT->lsibling!=NULL)
					{

						if(PT->lsibling->element==TK_INT)
							type=INT;
						else if(PT->lsibling->element==TK_REAL)
							type=REAL;
						else if(PT->lsibling->element==TK_RECORDID)
							type=RECORD;
						symbol_table_entry * temp= Symbol_table[nofscopes][hash_index];
	//					printf("TEMP %d\n",(temp==NULL)?0:1);
						while(temp!=NULL && strcmp(temp->tokenData.value, PT->termToken.value)!=0)
						{
							temp=(symbol_table_entry*)temp->next_token;
						}
						if(temp!=NULL)
						{
							temp->type=type;
							PT->type=type;
							if(PT->lsibling->element==TK_RECORDID)
								strcpy(temp->recordvalue, PT->lsibling->termToken.value);
						}
					}
				}
			}
			else{
				ret=searchIndex(PT->termToken, nofscopes, hash_index);
				if(ret<0)
				{
					//CHECK IN GLObAL SCOPE
					int ret1=searchIndex(PT->termToken, 0, hash_index);
					if(ret1<0)
					{
						//ERROR NOT DECLARED
						printf("-----------------------error not declared variable2 %s ----------\n", PT->termToken.value);
					}
					else
					{
						PT->termToken.index=hash_index*MAX_CLASH + ret1;
						PT->scope_no=0;
						symbol_table_entry * temp= Symbol_table[0][hash_index];
						int gen_j=0;
						while(gen_j<ret)
						{
							temp=(symbol_table_entry*)temp->next_token;
							gen_j++;
						}
						PT->type=temp->type;
					}
				}
				else
				{
					PT->termToken.index=hash_index*MAX_CLASH + ret;
					PT->scope_no=nofscopes;
					symbol_table_entry * temp= Symbol_table[nofscopes][hash_index];
					int gen_j=0;
					while(gen_j<ret)
					{
						temp=(symbol_table_entry*)temp->next_token;
						gen_j++;
					}
					PT->type=temp->type;
				}
			}

		}

		//Assign types from symbol table to ast itself.
		if(PT->termToken.index>=0)
		{
			symbol_table_entry* tmp_entry=Symbol_table[PT->scope_no][(PT->termToken.index)/MAX_CLASH];
			int indx=(PT->termToken.index)%MAX_CLASH;
			#ifdef DEBUG_SYMBOL_TABLE
			printf("here %d for %s\n", PT->termToken.index, PT->termToken.value);
			#endif
			while(tmp_entry!=NULL && indx--)
			{
				tmp_entry=(symbol_table_entry*)tmp_entry->next_token;
			}
			if(tmp_entry!=NULL)
			{
				PT->type=tmp_entry->type;
//				PT->scope_no=nofscopes;
			}
		}

		//if(PT->children!=NULL)
		PT=PT->children;

		while(PT!=NULL)
		{
			makeSymbolTable(PT);
			//if(PT!=NULL)
			{
				if(PT->sibling==NULL)break;
				PT=PT->sibling;
			}
		}

	}
}

void makeSymbolTable2(parseTree PT)
{
	//if(PT->children!=NULL)
	if(PT!=NULL)
	{

		if(PT!=NULL && (PT->element==TK_FUNID || PT->element==TK_MAIN))
		{

			unsigned int hash_index=0;
			int ret=0;
			hash_index=hash_symbol_table(PT->termToken.value);
			#ifdef DEBUG_SYMBOL_TABLE
			printf(" Function name: Hash index is: %d for string: %s at GLOBAL SCOPE\n", hash_index, PT->termToken.value);
			#endif
			if(PT->element==TK_FUNID && (PT->parent->element==otherFunctions+NO_OF_TERMINALS||PT->parent->element==program+NO_OF_TERMINALS)  ||(PT->element==TK_MAIN && PT->parent->element==mainFunction+NO_OF_TERMINALS))
			{
				#ifdef DEBUG_SYMBOL_TABLE
				printf("Function changing to%s\n", PT->termToken.value);
				#endif
				ret=insertinSymbolTable(PT->termToken, 0, hash_index);

				if(ret<0)
				{
					//ERROR REDECLARATION OF VARIABLE
					printf("-----------------------error redeclaration of FUNCTION %s ----------\n", PT->termToken.value);
				}
				else
				{
					PT->termToken.index=hash_index*MAX_CLASH + ret;
					PT->scope_no=0;
				}

			}
			else{
				ret=searchIndex(PT->termToken, 0, hash_index);
				if(ret<0)
				{
					//ERROR FUNCTION NOT DECLARED
					printf("-----------------------error not declared FUNCTION %s ----------\n", PT->termToken.value);
				}
				else
				{
					PT->termToken.index=hash_index*MAX_CLASH + ret;
					PT->scope_no=0;
				}
			}
		}
		if(PT!=NULL && PT->termToken.name==TK_ID && PT->sibling!=NULL && PT->sibling->element==TK_GLOBAL)
		{

			unsigned int hash_index=0;
			int ret=0;
			hash_index=hash_symbol_table(PT->termToken.value);
			#ifdef DEBUG_SYMBOL_TABLE
			printf("GLOBAL VARIABLE: Hash index is: %d for string: %s\n", hash_index, PT->termToken.value);
			#endif
			if(PT->parent->element==declaration+NO_OF_TERMINALS)
			{

				ret=insertinSymbolTable(PT->termToken, 0, hash_index);
				if(ret<0)
				{
					//ERROR REDECLARATION OF VARIABLE
					printf("-----------------------error redeclaration of GLOBAL variable %s----------\n", PT->termToken.value);
				}
				else
				{
					PT->termToken.index=hash_index*MAX_CLASH + ret;
					PT->scope_no=0;
				}
				int type=-1;
				if(PT->lsibling!=NULL)
				{

					if(PT->lsibling->element==TK_INT)
						type=INT;
					else if(PT->lsibling->element==TK_REAL)
						type=REAL;
					else if(PT->lsibling->element==TK_RECORDID)
						type=RECORD;
					symbol_table_entry * temp= Symbol_table[0][hash_index];
//					printf("TEMP %d\n",(temp==NULL)?0:1);
					while(temp!=NULL && strcmp(temp->tokenData.value, PT->termToken.value)!=0)
					{
						temp=(symbol_table_entry*)temp->next_token;
					}
					if(temp!=NULL)
					{
						temp->type=type;
						PT->type=type;
						if(PT->lsibling->element==TK_RECORDID)
							strcpy(temp->recordvalue, PT->lsibling->termToken.value);
					}
				}
			}

		}
		if(PT!=NULL && PT->termToken.name==TK_FIELDID)
		{
			unsigned int hash_index=0;
			int ret=0;
			hash_index=hash_symbol_table(PT->termToken.value);
			#ifdef DEBUG_SYMBOL_TABLE
			printf("GLOBAL RECORD: Hash index is: %d for string: %s\n", hash_index, PT->termToken.value);
			#endif
			if(PT->parent->element==fieldDefinition+NO_OF_TERMINALS|| PT->parent->element==parameter_list+NO_OF_TERMINALS)
			{
				ret=insertinSymbolTable(PT->termToken, 0, hash_index);
				if(ret<0)
				{
					//ERROR REDECLARATION OF VARIABLE
					printf("-----------------------error redeclaration of GLOBAL FIELD variable %s----------\n", PT->termToken.value);
				}
				else
				{
					PT->termToken.index=hash_index*MAX_CLASH + ret;
					PT->scope_no=0;
				}
				int type=-1;
				if(PT->lsibling!=NULL)
				{
					if(PT->lsibling->element==TK_INT)
						type=INT;
					else if(PT->lsibling->element==TK_REAL)
						type=REAL;
					else if(PT->lsibling->element==TK_RECORDID)
						type=RECORD;
					symbol_table_entry * temp= Symbol_table[0][hash_index];
//					printf("TEMP %d\n",(temp==NULL)?0:1);
					while(temp!=NULL && strcmp(temp->tokenData.value, PT->termToken.value)!=0)
					{
						temp=(symbol_table_entry*)temp->next_token;
					}
					if(temp!=NULL)
					{	temp->type=type;
						PT->type=type;
						strcpy(temp->recordvalue, recordval);
					}
				}
			}
		}
		PT=PT->children;

		while(PT!=NULL)
		{
			makeSymbolTable2(PT);
			//if(PT!=NULL)
			{
				if(PT->sibling==NULL)break;
				PT=PT->sibling;
			}
		}

	}
}


//SYMBOL TABLE FUNCTIONS END


void makeSymbolTableRoot(parseTree PT)
{
	if(PT!=NULL)
	{
		makeSymbolTable2(PT);
		if(PT->children!=NULL)
			makeSymbolTable(PT->children->sibling);
		makeSymbolTable(PT->children);
	}

}



//AST FUNCTIONS


void makeASTtable()
{

	int l,k;
	for(k=0;k<NO_OF_GRAMMAR_RULES;k++)
		for(l=0;l<10;l++)
			ast_rules[k][l]=-1;

	FILE * ast_file = fopen("AST_RULES.txt","rw");
	int i=0, eflag=0;
	char t1;


	while(!feof(ast_file))
	{
		t1='a';
/*		while(t1!='\n'&& !feof(ast_file))*/
/*			fscanf(ast_file,"%c",&t1);*/
		char t2[1024];
		fgets(t2,1000, ast_file);

		char action[1000], term[1000];

		int count=0;

		fscanf(ast_file,"%s %s",action, term);

		while((action[0]=='N'||action[0]=='P'||action[0]=='D'))
		{
			if(action[0]=='N')
				ast_rules[i][count++]=0;
			else if(action[0]=='P')
				ast_rules[i][count++]=1;
			else if(action[0]=='D')
				ast_rules[i][count++]=2;

			if(feof(ast_file))
			{
				eflag=1;
				break;
			}

			fscanf(ast_file,"%s %s",action, term);
		}

		int j=0;
		for(j=count;j<10;j++)
		{
			ast_rules[i][j]=-1;
		}
		if(eflag==1)
		{
			break;
		}
		i++;

	}
	fclose(ast_file);
	k=l=0;

//	Print RULES
//	for(k=0;k<90;k++)
//	{	printf("Rule %d\t", k);
//		for(l=0;l<10;l++)
//			printf("%d\t",ast_rules[k][l]);
//		printf("\n");
//	}

}


void makeAST(parseTree * PT)
{
	if(*PT!=NULL)
	{
//		printf("function working for: %d\n", (*PT)->element);

		int size=(*PT)->listSize;
		int rule=(*PT)->ruleUsed;
		int i=0;
		parseTree temp= *PT;
		temp=temp->children;

		while(size-->0)
		{
			makeAST(&temp);
			if(temp!=NULL)
			{
//				printf("choice is %d\n",ast_rules[rule][i]);

				if(ast_rules[rule][i]==2)
				{
					#ifdef DEBUG_AST
					printf("delete element %d\n",(temp)->element);
					#endif
					if(temp->sibling!=NULL)
						temp->sibling->lsibling=temp->lsibling;
					else if(temp->lsibling!=NULL)
						temp->lsibling->sibling=NULL;

					if(temp->lsibling!=NULL)
						temp->lsibling->sibling=temp->sibling;
					else if(temp->sibling!=NULL)
						temp->sibling->lsibling=NULL;


					if((*PT)->children==temp)
						(*PT)->children=temp->sibling;

					(*PT)->listSize--;
					free(temp);
				}
				if(ast_rules[rule][i]==1)
				{
					#ifdef DEBUG_AST
					printf("pullup element %d parent %d size %d\n",(temp)->element, (*PT)->element, (*PT)->listSize);
					#endif
					parseTree tempchild=temp->children;
//					copyNode((*PT),temp);

					(*PT)->element=temp->element;
					(*PT)->ruleUsed=temp->ruleUsed;
					(*PT)->termToken=temp->termToken;
					(*PT)->scope_no= temp->scope_no;
					(*PT)->type= temp->type;

					if((*PT)->children==temp)
					{
						if(tempchild!=NULL)
							(*PT)->children=tempchild;
						else
						{
							(*PT)->children=temp->sibling;
							(*PT)->listSize--;
						}
						parseTree t1=(*PT)->children;
						parseTree t2=NULL;
						while(t1!=NULL)
						{
							t1->parent=(*PT);
							t2=t1;
							t1=t1->sibling;
						}
						if(temp->sibling!=NULL&& tempchild!=NULL)
						{
							t2->sibling=temp->sibling;
							temp->sibling->lsibling=t2;
						}



					}
					else
					{
						parseTree t2=temp,t3;
						while(t2!=NULL)
						{
							t3=t2;
							t2=t2->lsibling;
						}

						(*PT)->children=t3;
						(*PT)->listSize--;
						parseTree t1=t3;
						while(t1!=NULL)
						{
							t1->parent=(*PT);
							t1=t1->sibling;
						}


						if(temp->sibling!=NULL)
							temp->sibling->lsibling=temp->lsibling;
						else if(temp->lsibling!=NULL)
							temp->lsibling->sibling=NULL;
						else (*PT)->children=NULL;


						if(temp->lsibling!=NULL)
							temp->lsibling->sibling=temp->sibling;
						else if(temp->sibling!=NULL)
							temp->sibling->lsibling=NULL;
						else (*PT)->children=NULL;
					}
					free(temp);
				}
				else if(ast_rules[rule][i]==-1)
				{
					printf("ERROR: RULE USED IS -1 rule %d count %d\n", rule, i);
				}
				temp=temp->sibling;
			}
			i++;
		}

		parseTree checkChildren=*PT,deleteChildren=*PT,tempDeleteChildren=*PT;
		checkChildren=checkChildren->children;
		int childCount=0, childEpsilonCount=0;
		while(checkChildren!=NULL)
		{
			if(checkChildren->element==EPSILON)// && deleteChildren->termToken.name==EPSILON)
				childEpsilonCount++;
			else	childCount++;
			checkChildren=checkChildren->sibling;
		}

		#ifdef DEBUG_AST

		printf("CHILDCOUNT %d CHILDEPSILONCOUNT %d element %d\n", childCount, childEpsilonCount, (*PT)->element);
		if(childCount==2 && childEpsilonCount==0)
			printf("left= %d right = %d\n", (*PT)->children->element, (*PT)->children->sibling->element);
		#endif

		if(childCount==1 && childEpsilonCount>0 && (*PT)->element>=NO_OF_TERMINALS)
		{
			#ifdef DEBUG_AST
			printf("DELETING EPSILONS AND PULLING UP at %d line %d\n", (*PT)->element, (*PT)->termToken.lineNumber);
			#endif

			deleteChildren=deleteChildren->children;

			while(deleteChildren!=NULL)
			{
				if(deleteChildren->element==EPSILON )
				{
					#ifdef DEBUG_AST
					printf("delete element %d\n",(deleteChildren)->element);
					#endif

					if(deleteChildren->sibling!=NULL)
						deleteChildren->sibling->lsibling=deleteChildren->lsibling;
					else if(deleteChildren->lsibling!=NULL)
						deleteChildren->lsibling->sibling=NULL;

					if(deleteChildren->lsibling!=NULL)
						deleteChildren->lsibling->sibling=deleteChildren->sibling;
					else if(deleteChildren->sibling!=NULL)
						deleteChildren->sibling->lsibling=NULL;


					if((*PT)->children==deleteChildren)
						(*PT)->children=deleteChildren->sibling;

					(*PT)->listSize--;
					tempDeleteChildren=deleteChildren;
					free(tempDeleteChildren);
				}

				deleteChildren=deleteChildren->sibling;
			}
			#ifdef DEBUG_AST
			printf("PULLING UP...%d\n", (*PT)->children->element);
			#endif
			deleteChildren=(*PT)->children;
			(*PT)->element=deleteChildren->element;
			(*PT)->ruleUsed=deleteChildren->ruleUsed;
			(*PT)->termToken=deleteChildren->termToken;
			(*PT)->type=deleteChildren->type;
			(*PT)->listSize=0;
			(*PT)->children=NULL;
			(*PT)->scope_no=deleteChildren->scope_no;
			(*PT)->children=deleteChildren->children;
			parseTree editParent= deleteChildren->children;
/*			if(editParent->children!=NULL)*/
/*			{*/
/*				editParent=editParent->children;*/
				while(editParent!=NULL)
				{
					editParent->parent=(*PT);
					editParent=editParent->sibling;
				}
/*			}*/
			free(deleteChildren);
		}

	}
}

int typeCheck(parseTree PT)
{
	if(PT!=NULL)
	{
		PT=PT->children;
		while(PT!=NULL)
		{
			typeCheck(PT);
			if(PT->element==TK_ASSIGNOP)
			{
				#ifdef DEBUG_TYPECHECK
				printf("Assigning assignop type\n");
				#endif
				//LEFT IS TK_ID RIGHT IS NUM, RNUM, ID
				if(getListSize(PT)==2 && PT->children->element<NO_OF_TERMINALS)
				{
					if(PT->children->type!=PT->children->sibling->type)
					{
						//EXPRESSION TYPE DERIVATION ERROR
						printf("EXPRESSION TYPE DERIVATION ERROR\n");
						PT->type=(PT->children->type>PT->children->sibling->type?PT->children->type:PT->children->sibling->type);
					}
					else
					{
						if(PT->children->type==RECORD && PT->children->sibling->element==TK_ID)
						{
							symbol_table_entry* tmp_entry=Symbol_table[PT->children->scope_no][(PT->children->termToken.index)/MAX_CLASH];
							char rec[MAXSTRLEN];
							char rec1[MAXSTRLEN];
							int indx=(PT->children->termToken.index)%MAX_CLASH;
							//printf("start %s %d %d\n", PT->children->termToken.value, PT->children->termToken.index, PT->children->scope_no);

							while(tmp_entry!=NULL && indx--)
							{
								tmp_entry=(symbol_table_entry*)tmp_entry->next_token;
							}

							if(tmp_entry!=NULL)
							{
								strcpy(rec, tmp_entry->recordvalue);
							}

							symbol_table_entry* tmp_entry1=Symbol_table[PT->children->sibling->scope_no][(PT->children->sibling->termToken.index)/MAX_CLASH];

							int indx1=(PT->children->sibling->termToken.index)%MAX_CLASH;
							while(tmp_entry1!=NULL && indx1--)
							{
								tmp_entry1=(symbol_table_entry*)tmp_entry1->next_token;
							}
							if(tmp_entry1!=NULL)
							{
								strcpy(rec1, tmp_entry1->recordvalue);
							}
							if(strcmp(rec, rec1)!=0)
							{
								printf("ERROR: Record types not same %s vs %s for %d vs %d\n", rec, rec1, PT->children->element, PT->children->sibling->element);
							}

						}
					}
				}
			}
			else if(PT->element==NO_OF_TERMINALS+SingleOrRecId)
			{
				#ifdef DEBUG_TYPECHECK
				printf("Assigning singleOrRecId type\n");
				#endif
				PT->type=PT->children->sibling->type;

			}
			else if(PT->element==NO_OF_TERMINALS+arithmeticExpression || PT->element==NO_OF_TERMINALS+term || PT->element==TK_PLUS || PT->element==TK_MUL || PT->element==TK_MINUS || PT->element==TK_DIV)
			{
				#ifdef DEBUG_TYPECHECK
				printf("Assigning arith term plus minus mul div type\n");
				#endif
				if(PT->children->type!=PT->children->sibling->type)
				{
					//EXPRESSION TYPE DERIVATION ERROR
					printf("EXPRESSION TYPE DERIVATION ERROR %s %s %s %d %d\n", PT->termToken.value, PT->children->termToken.value,PT->children->sibling->termToken.value,PT->children->type,PT->children->sibling->type);
					PT->type=(PT->children->type>PT->children->sibling->type?PT->children->type:PT->children->sibling->type);
				}
				else{
					PT->type=PT->children->type;
				}

			}
			else if(PT->element>=TK_AND && PT->element<=TK_NOT)
			{
				if(PT->children->type!=PT->children->sibling->type || PT->children->type!=BOOL)
				{
					//EXPRESSION TYPE DERIVATION ERROR
					printf("EXPRESSION TYPE DERIVATION ERROR %s %s %s %d %d\n", PT->termToken.value, PT->children->termToken.value,PT->children->sibling->termToken.value,PT->children->type,PT->children->sibling->type);

				}
				PT->type=BOOL;
			}
			else if(PT->element>=TK_LT && PT->element<=TK_NE)
			{
				if(PT->children->type!=PT->children->sibling->type)
				{
					//EXPRESSION TYPE DERIVATION ERROR
					printf("EXPRESSION TYPE DERIVATION ERROR %s %s %s %d %d\n", PT->termToken.value, PT->children->termToken.value,PT->children->sibling->termToken.value,PT->children->type,PT->children->sibling->type);

				}
				PT->type=BOOL;
			}
			else if(PT->element==EPSILON && (PT->parent->element==TK_PLUS ||PT->parent->element==TK_MUL ||PT->parent->element==TK_MINUS ||PT->parent->element==TK_DIV))
			{
				#ifdef DEBUG_TYPECHECK
				printf("Assigning epsilon type parent %d\n", PT->parent->element);
				#endif
				PT->type=PT->lsibling->type;
			}
			else if(PT->element==TK_NUM)
			{
				#ifdef DEBUG_TYPECHECK
				printf("Assigning tknum type\n");
				#endif
				PT->type=INT;
			}
			else if(PT->element==TK_RNUM)
			{
				#ifdef DEBUG_TYPECHECK
				printf("Assigning rnum type\n");
				#endif
				PT->type=REAL;
			}

			PT=PT->sibling;
		}
	}
}

int hasRecursion(parseTree PT)
{

	if(PT!=NULL)
	{
		if(PT->element==TK_FUNID || PT->element==TK_MAIN)
		{
			if(PT->parent->element==otherFunctions+NO_OF_TERMINALS||PT->parent->element==program+NO_OF_TERMINALS||PT->parent->element==mainFunction+NO_OF_TERMINALS)
			{
				strcpy(funname, PT->termToken.value);
			}
			else
			{
				if(strcmp(funname,PT->termToken.value)==0)
				{
					//FUNCTION RECURSION ERRORS
					printf("----------------------------FUNCTION RECURSION ERROR in fnction: %s\n", funname);

				}
			}
		}
		PT=PT->children;
		while(PT!=NULL)
		{
			hasRecursion(PT);
			PT=PT->sibling;
		}
	}
}

int semanticCheck(parseTree PT)
{
		hasRecursion(PT);
}
