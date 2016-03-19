#include "parserDef.h"

extern extern void printfGrammar(grammar G);

extern void push(char *str, Stack* head);
extern void pop(Stack* head);
extern void initStack(Stack* s);
extern Token *getNextToken(FILE *f);
extern int get_t_index(char* str);
extern int get_nt_index(char* str);
extern void createParseTable();

extern parseTree parseInputSourceCode();
extern void printNonTerminal(parseTree curNode, FILE *fp);
extern void printNumber(parseTree curNode, FILE* fp);
extern void printGeneral(parseTree curNode, FILE* fp);
extern void printRecursive(parseTree curNode, FILE* fp);
extern void printParseTree(parseTree PT, char *pt_output);
extern void parserInitialize();

extern void initSymbolTable();
extern int insertSymbol(Token tk,int hash, int scope);
extern void printSymbol_Table(char *st_output);
extern int stringIndex(Token tok, int hash,int scope);
extern unsigned int hash_node(char *str);
extern void makeSymbolTable(parseTree PT);
extern void makeST2(parseTree PT);
extern void makeSTRoot(parseTree PT);

extern void deleteAstNode(parseTree pt,char str[30]);
extern void pullUpAstNode(parseTree pt,char str[30]);
extern parseTree makeAST(parseTree PT)