all: lexerDebug lexer

lexerDebug: regex.o value.o token.o
	g++ -std=c++11 -g regex.o value.o token.o -o lexerDebug

lexer: regex.o value.o token.o
	g++ -std=c++11 regex.o value.o token.o -o lexer

token.o: token.cpp
	g++ -std=c++11 -g -c token.cpp 

value.o: value.cpp
	g++ -std=c++11 -g -c value.cpp 

regex.o: regex.cpp 
	g++ -std=c++11 -g -c regex.cpp

clean:
	rm *.o lexer lexerDebug