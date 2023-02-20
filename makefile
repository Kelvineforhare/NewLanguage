all: lexerDebug lexer

lexerDebug: regex.o value.o main.o
	g++ -std=c++11 -g regex.o value.o main.o -o lexerDebug

lexer: regex.o value.o main.o
	g++ -std=c++11 regex.o value.o main.o -o lexer

main.o: main.cpp
	g++ -std=c++11 -g -c main.cpp 

value.o: value.cpp
	g++ -std=c++11 -g -c value.cpp 

regex.o: regex.cpp 
	g++ -std=c++11 -g -c regex.cpp

clean:
	rm *.o lexer lexerDebug