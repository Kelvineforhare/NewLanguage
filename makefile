lexer: value.o regex.o
	g++ -std=c++11 value.o regex.o -o lexer

value.o: value.cpp 
	g++ -std=c++11 -c value.cpp

regex.o: regex.cpp 
	g++ -std=c++11 -c regex.cpp

clean:
	rm *.o lexer