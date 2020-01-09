.PHONY = all clean cleanall
FLAGS = -std=c++17 --pedantic -Wextra -Wall

all: cleanall compiler

compiler: parser lexer data code types
	g++ $(FLAGS) -o compiler parser.o lexer.o data.o code.o types.o

types: types.cpp
	g++ $(FLAGS) -c types.cpp -o types.o

code: code.cpp
	g++ $(FLAGS) -c code.cpp -o code.o

data: data.cpp
	g++ $(FLAGS) -c data.cpp -o data.o

lexer: lexer.c
	g++ $(FLAGS) -o lexer.o -c lexer.c

lexer.c: lexer.l
	flex -o lexer.c lexer.l

parser: parser.tab.cpp
	g++ $(FLAGS) -o parser.o -c parser.tab.cpp

parser.tab.cpp: parser.ypp
	bison -d parser.ypp

clean:
	rm -f *.o *.tab.cpp *.tab.hpp lexer.l.*

cleanall: clean
	rm -f compiler
