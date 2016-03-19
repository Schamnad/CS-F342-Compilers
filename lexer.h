/*
COMPILER PROJECT-2015
Batch Number: 11
2011B2A7744P Sanjeed Schamnad
2011A7PS479P	Shiva Manne
*/

#if ! defined _ldef_H
#define _ldef_H 1

#include"lexerDef.h"

#endif

extern void init();
extern int getIndex(char ch);
extern int populate_DFA_table(FILE *dfaFile);
extern tokenInfo getNextToken();
extern tokenInfo getStream(FILE *instream);
extern void removeComments(char *filename);
