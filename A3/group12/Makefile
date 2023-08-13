CXX = g++-8
EXE = iplC
CXXDEBUG = -g -Wall
CXXSTD = -std=c++11


.PHONY: all
all: parser lexer main.cpp type.cpp symboltable.cpp ast.cpp
	$(CXX) $(CXXDEBUG) $(CXXSTD) -o $(EXE)  main.cpp type.cpp symboltable.cpp ast.cpp parser.o scanner.o 

parser: parser.yy
	bison -d -v $<
	$(CXX) $(CXXDEBUG) $(CXXSTD) -c parser.tab.cc -o parser.o 

lexer: scanner.l scanner.hh
	flex++ --outfile=scanner.yy.cc  $<
	$(CXX)  $(CXXDEBUG) $(CXXSTD) -c scanner.yy.cc -o scanner.o

clean:
	rm $(EXE) parser.output parser.tab.cc stack.hh scanner.yy.cc parser.o scanner.o parser.tab.hh location.hh position.hh
