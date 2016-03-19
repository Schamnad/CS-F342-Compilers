/*
COMPILER PROJECT-2015
Batch Number: 11
2011B2A7744P Sanjeed Schamnad
2011A7PS479P	Shiva Manne
*/

#define ERROR_STATE 189
#define ERROR_ACCEPT 190

#define BUFFER_SIZE 50
#define STATE_SIZE 191
#define ALPHABET_SIZE 85
#define MAXSTRLEN 100

typedef enum{false=0, true=1} BOOLEAN;

typedef enum {EPSILON=0, TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RECORDID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_INT, TK_REAL, TK_TYPE, TK_MAIN, TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE, TK_COMMA, $, TK_ERROR} terminal;

typedef enum {program=0, otherFunctions, mainFunction, stmts, function, input_par, output_par, parameter_list, dataType, remaining_list, primitiveDatatype, constructedDatatype, typeDefinitions, declarations, returnStmt, typeDefinition, fieldDefinitions, fieldDefinition, moreFields, declaration, global_or_not, stmt, otherStmts, assignmentStmt, iterativeStmt, conditionalStmt, ioStmt, funCallStmt, SingleOrRecId, arithmeticExpression, outputParameters, inputParameters, idList, booleanExpression, elsePart, allVar, var, term, expPrime, lowPrecenceOperators, factor, termPrime, highPrecedenceOperators, all, temp, operator, logicalOp, relationalOp, optionalReturn, more_ids,singleOrRecIdPrime} nonTerminal;


typedef struct
{
	int index;
	terminal name;
	char value[MAXSTRLEN];
	int lineNumber;
}tokenInfo;


