.PHONY = all clean cleanall
FLAGS = -std=c++17 --pedantic -Wextra -Wall

all: compiler

compiler: parser lexer data code types
	g++ $(FLAGS) -o kompilator parser.o lexer.o data.o code.o program.o command.o condition.o expression.o value.o identifier.o instruction.o

types: program.cpp command.cpp condition.cpp expression.cpp value.cpp identifier.cpp instruction.cpp
	g++ $(FLAGS) -c program.cpp -o program.o
	g++ $(FLAGS) -c command.cpp -o command.o
	g++ $(FLAGS) -c condition.cpp -o condition.o
	g++ $(FLAGS) -c expression.cpp -o expression.o
	g++ $(FLAGS) -c value.cpp -o value.o
	g++ $(FLAGS) -c identifier.cpp -o identifier.o
	g++ $(FLAGS) -c instruction.cpp -o instruction.o

code: code.cpp
	g++ $(FLAGS) -c code.cpp -o code.o

data: data.cpp
	g++ $(FLAGS) -c data.cpp -o data.o

lexer: lexer.c
	g++ $(FLAGS) -o lexer.o -c lexer.c

parser: parser.tab.cpp
	g++ $(FLAGS) -o parser.o -c parser.tab.cpp

lexer.c: lexer.l
	flex -o lexer.c lexer.l

parser.tab.cpp: parser.ypp
	bison -d parser.ypp

clean:
	rm -f *.o *.tab.cpp *.tab.hpp lexer.c

cleanall: clean
	rm -f compiler

test:
	cd jftt2019-testy && python3.8 tester.py
