lexer: regex.o value.o
	g++ -std=c++11 regex.o value.o -o lexer

regex.o: regex.cpp 
	g++ -std=c++11 -c regex.cpp

value.o: value.cpp 
	g++ -std=c++11 -c value.cpp

clean:
	rm *.o lexer