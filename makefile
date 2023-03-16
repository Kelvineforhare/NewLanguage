armin: regex.o value.o token.o main.o
	g++ -std=c++11 regex.o value.o token.o main.o -o armin

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

token.o: token.cpp
	g++ -std=c++11 -c token.cpp 

value.o: value.cpp
	g++ -std=c++11 -c value.cpp 

regex.o: regex.cpp 
	g++ -std=c++11 -c regex.cpp

clean:
	rm *.o armin