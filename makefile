lexer: lexer.o
	g++ -std=c++11 lexer.o -o lexer

lexer.o: lexer.cpp 
	g++ -std=c++11 -c lexer.cpp

clean:
	rm *.o lexer