/*
COMPILER PROJECT-2015
Batch Number: 11
2011B2A7744P Sanjeed Schamnad
2011A7PS479P	Shiva Manne
*/

#if ! defined _pdef_H
#define _pdef_H 1

#include"parserDef.h"

#endif
extern STACK initStack(void);
extern STACK_ELEM getStackNode();
extern void init (STACK* Top, int n);
extern void push (STACK* Top, int a);
extern BOOLEAN pop (STACK* Top, int* a);
extern BOOLEAN top (STACK* Top, int* a);
extern BOOLEAN empty(STACK Top); 
extern void display(STACK Top); 
extern int count(STACK Top); 


extern parseTree initParseTree();
extern treeNode_element getNewNode();
extern int insertInto(treeNode_element* P, int a);
extern int printParseTree(parseTree  PT, char* outfile);
extern void printParseTreeRecur(parseTree  PT, FILE *fp);
extern int initialize_first_follow_sets();
extern void populate_parse_table();
extern void print_sets();
extern int parse_this(FILE *instream, parseTree * Parse_tree);
extern parseTree copyParseTree(parseTree fromPT, parseTree toPT);
extern void printTree(parseTree PT);

extern unsigned int hash_symbol_table(char *str);
extern symbol_table_entry* getNewSymbolTableEntry();
extern int insertinSymbolTable(tokenInfo tok, int scope_no, int hashed_ind);
extern void print_symbol_table();
extern void makeSymbolTableRoot(parseTree PT);
extern void makeSymbolTable(parseTree PT);

extern void makeAST(parseTree * PT);
extern void makeASTtable();

extern int semanticCheck(parseTree PT);
