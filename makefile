all: lexer.o parser.o driver.c
	gcc parser.o lexer.o -o stage1exe driver.c
parser.o: parser.c parser.h parserDef.h
	gcc -c parser.c
lexer.o: lexer.c lexer.h lexerDef.h
	gcc -c lexer.c
clean: 
	rm *o stage1exe
