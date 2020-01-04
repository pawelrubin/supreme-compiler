all: parser.tab lexer.l.c
	g++ -o parser parser.tab.cpp lexer.l.c

lexer.l.c: lexer.l
	flex -o lexer.l.c lexer.l

parser.tab: parser.ypp
	bison -d parser.ypp

clean:
	rm parser.tab.* lexer.l.*

cleanall: clean
	rm parser
