/*
COMPILER PROJECT-2015
Batch Number: 11
2011B2A7744P Sanjeed Schamnad
2011A7PS479P	Shiva Manne
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#if ! defined _l_H
#define _l_H 1

#include"lexer.h"

#endif


int DFA_table[STATE_SIZE][ALPHABET_SIZE];
terminal *FINAL[STATE_SIZE];


char temp_val[MAXSTRLEN];		//STORING THE LEXEME


int LINE_NUMBER=0;

// Lexical Analysis Variables
char PREVLOOKAHEAD='\0';
char mainBuffer[BUFFER_SIZE];
int BUFFER_POS=0;
int moreBuffer=0;
int PRESENT_STATE=0;
int LOOKAHEAD_INDEX=0;
int REPEAT_COUNTER_FUNID=0,REPEAT_COUNTER_ID=0;
int ERROR_NUMBER=0;
int val_pos=0;
BOOLEAN readMore=1;
BOOLEAN FINAL_STATE=false;
// end


void init()
{
	int i=0, j=0;
	for(i=0;i<STATE_SIZE;i++)
	{
		for(j=0; j<ALPHABET_SIZE; j++)
		{
			DFA_table[i][j]=-1;
		}
		FINAL[i]=NULL;
	}	
}


int getIndex(char ch)
{
	if(ch<='9' && ch>='0')
		return ch-'0';
	else if(ch>='A' && ch<='Z')
		return ch-'A'+10;
	else if(ch>='a' && ch<='z')
		return ch-'a'+10+26;
	else if(ch==' ' || ch== '\n' || ch== '\t')
		return 62;
	else if(ch=='!')
		return 63;	
	else if(ch=='#')
		return 64;	
	else if(ch=='&')
		return 65;	
	else if(ch>='(' && ch<='/')
		return ch-'('+10+26+26+4;
	else if(ch>=':' && ch <='>')
		return ch-':'+10+26+26+4+8;
	else if(ch=='@')
		return 79;
	else if(ch=='[')
		return 80;
	else if(ch==']')
		return 81;
	else if(ch=='_')
		return 82;
	else if(ch=='~')
		return 83;
	else if(ch=='%')
		return 84;	
	else
		return -1;
}


int populate_DFA_table(FILE *dfaFile)
{
	init();
	int stateNum=0, i=0, transNum=0;
	fscanf(dfaFile, "%d", &stateNum);
	int stateCount=stateNum;
	int startIndex=-1, endIndex=-1,finalState=-1, presentState=-1;
	char inputChar;	
	char temp;
	int terminalNum=-1;
	while(stateNum--)
	{
		fscanf(dfaFile, "%d", &presentState);
		
		if(presentState==-1)			
		{
			fscanf(dfaFile, "%c", &temp);
			continue;
		}
		
		fscanf(dfaFile, "%d", &transNum);
		if(transNum!=-1)
		{
			while(transNum--)
			{
				fscanf(dfaFile, "\n%c ", &inputChar);
				fscanf(dfaFile, "%d", &finalState);

				if(inputChar=='?') //ALL ALPHABETS
				{
					startIndex = getIndex('A');
					endIndex = getIndex('z');
				}
				else if(inputChar == '|') //NUMBERS
				{
					startIndex= getIndex('0'); //moreBuffer= BUFFER_POS;
					endIndex = getIndex('9');
				}
				else if(inputChar == '$') //SYMBOLS
				{
					startIndex=getIndex(' ');
					endIndex= getIndex('~');
				}
				else if(inputChar == '"') //WHITESPACE
				{
					startIndex=getIndex(' ');
					endIndex= getIndex(' ');
				}
				else if(inputChar == '^') //a-z
				{
					startIndex=getIndex('a');
					endIndex= getIndex('z');
				}
				else if(inputChar == 125)// ALL CHAR // 125 = ascii(})
				{
					startIndex=getIndex('0');
					endIndex= getIndex('~');
				}
				else
				{
					startIndex = endIndex = getIndex(inputChar);
				}				
				for(i=startIndex; i<=endIndex; i++)
				{
					if(i!=-1)
					{
						DFA_table[presentState][i]=finalState;
					}
					else
					{
						printf("\nERROR: UNKNOWN SYMBOL: %c\n in DFA file\n", inputChar);
						exit(0);
					}
				}
			}
		}
		else 																			//for final states
		{
			FINAL[presentState]=(terminal*)malloc(sizeof(terminal));
			fscanf(dfaFile, "%d",  &terminalNum);
			*FINAL[presentState]=terminalNum;
		}
	}
/*	for(i=0; i<=STATE_SIZE; i++)*/
/*	{*/
/*		if(FINAL[i]!=NULL)*/
/*			//if(*FINAL[i]==true)*/
/*				printf("%d\n",i);*/
/*	}*/
}


tokenInfo getNextToken()
{
	char lookahead;
	int table_val, index1;
	int cpy1=0;
	
	tokenInfo Next_token;
	readMore=0;
	FINAL_STATE=false;
	while( mainBuffer[BUFFER_POS]!='\0' )
	{
		if(moreBuffer == BUFFER_SIZE-2 && PREVLOOKAHEAD!=' ' && PREVLOOKAHEAD!='\n')
		{			
			lookahead=PREVLOOKAHEAD;
			BUFFER_POS--;
			moreBuffer=0;
		}
		else
			lookahead=mainBuffer[BUFFER_POS];
			
		index1=getIndex(lookahead);
		if(index1!=-1)
			table_val=DFA_table[PRESENT_STATE][index1];
		else
			table_val=-1;
			
			
		if(table_val!=-1)
		{
			PRESENT_STATE=table_val;
			if(PRESENT_STATE==26 || PRESENT_STATE==25)
			{
				REPEAT_COUNTER_FUNID=0;
				REPEAT_COUNTER_ID++;
				if(REPEAT_COUNTER_ID<4)
					REPEAT_COUNTER_ID=strlen(temp_val);
				if(REPEAT_COUNTER_ID>21)
				{
					PRESENT_STATE=ERROR_STATE;		
					ERROR_NUMBER=5;
				}
			
			}
			else if (PRESENT_STATE==17)
			{
				REPEAT_COUNTER_ID=0;
				if(REPEAT_COUNTER_FUNID<6)
					REPEAT_COUNTER_FUNID=strlen(temp_val);
				REPEAT_COUNTER_FUNID++;
				if(REPEAT_COUNTER_FUNID>30)
				{
					PRESENT_STATE=ERROR_STATE;		
					ERROR_NUMBER=6;
				}

			}
			else{
				REPEAT_COUNTER_ID=0;
				REPEAT_COUNTER_FUNID=0;
			}

		}
		else
		{
			PRESENT_STATE=ERROR_STATE;
			temp_val[val_pos]='\0';
			if(index1==-1)ERROR_NUMBER=2;
			else
				ERROR_NUMBER=3;
		}	
		int *ret;
		
		if(FINAL[PRESENT_STATE] != NULL)
		{
			temp_val[val_pos]='\0';				
			if(ERROR_NUMBER==5)
			{
				printf("ERROR_1: Length of Identifier %s at line %d is too long\n", temp_val, LINE_NUMBER+1);
				ERROR_NUMBER=0;
			}
			else if(ERROR_NUMBER==6)
			{
				printf("ERROR_1: Length of Functional Identifier %s at line %d is too long\n", temp_val, LINE_NUMBER+1);
				ERROR_NUMBER=0;
			}
			else if(ERROR_NUMBER==2)
			{
				printf("ERROR_2: Unknown Symbol  %s at line %d\n", temp_val, LINE_NUMBER+1);
				ERROR_NUMBER=0;
			}
			else if(ERROR_NUMBER==3)
			{
				printf("ERROR_3: Unknown pattern %s at line %d\n", temp_val, LINE_NUMBER+1);
				ERROR_NUMBER=0;
			}
			
				
			moreBuffer= BUFFER_POS;

			Next_token.name = *(FINAL[PRESENT_STATE]);
			strcpy(Next_token.value, temp_val);

			Next_token.index=-1;
			Next_token.lineNumber=LINE_NUMBER+1;
		
			val_pos=0;
			int loop_k=0;
			for(loop_k=0; loop_k<MAXSTRLEN; loop_k++)
				temp_val[loop_k]='\0';
				
			FINAL_STATE=true;
			PRESENT_STATE=0;
			
			if(moreBuffer== BUFFER_SIZE-2)
				PREVLOOKAHEAD=lookahead;
		
			return Next_token;
		}
		else
			if(lookahead=='\n' && PREVLOOKAHEAD!='\n')
			LINE_NUMBER++;
		
		if( getIndex(lookahead)!=62 )
			temp_val[val_pos++]=lookahead;

		BUFFER_POS++;
		FINAL_STATE=false;
	}
	readMore=1;

}


tokenInfo getStream(FILE *instream)
{
	int res1=-1;
	int begin=0;

	tokenInfo next_token_read, prev_token_read;
	if(readMore==1)
	{
	res1=fread(mainBuffer, 1, BUFFER_SIZE-1, instream);
	mainBuffer[res1]='\0';		
	}
	while(res1!=0)
	{
		prev_token_read = next_token_read;

		next_token_read = getNextToken();
		
		if(next_token_read.index==-9)
			return next_token_read;

		if(BUFFER_POS==BUFFER_SIZE-2 || readMore==1)
		{
			res1=fread(mainBuffer, 1, BUFFER_SIZE-1, instream);
			BUFFER_POS=0;
			readMore=0;
			mainBuffer[res1]='\0';		
		}
	
		next_token_read.index=-1;
		if(FINAL_STATE==true)
			return next_token_read;
	}
	terminal last=$;
	tokenInfo last1;
	last1.index=-1;
	last1.name=last;
	strcpy(last1.value,"$");
//	fprintf(token_stream, "%d\n", last);
//	fclose(token_stream);
	printf("\n");
	return last1;
}

void removeComments(char * filename)
{
	char * cleaned_file,newFile[10000];
	char str[200];
	
	FILE* fp=fopen(filename,"r");

	if(!fp) return (void)NULL;
	while(fgets(str,sizeof(str),fp) != NULL)
	{
	   	strcat(newFile,str);
	}
	FILE *comments_rem=fopen("noComments.txt", "w");
	int i=0, j=0, flag=0;
	int *ret=0;
	cleaned_file=(char*)malloc(sizeof(char));
	
	while(i<strlen(newFile))
	{
		if(flag==0)
		{
			if(newFile[i] =='%')
			{
				flag=1;
				i++;
				continue;		
			}
			else
			{
				ret=realloc(cleaned_file, (j+1)*sizeof(char));
				cleaned_file[j++]=newFile[i];
				i++;
		
			}
		}
		else
		{
			if(newFile[i] =='\n')
			{
				ret=realloc(cleaned_file, (j+1)*sizeof(char));
				cleaned_file[j++]='\n';
				flag=0;
				i++;
			}
			else
				i++;		
		}
	}
	ret=realloc(cleaned_file, (j+1)*sizeof(char));
	cleaned_file[j]='\0';
	fprintf(comments_rem, "%s\n\n", cleaned_file);
	fclose(comments_rem);
}
