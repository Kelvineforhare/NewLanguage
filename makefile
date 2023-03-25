armin: regex.o value.o token.o main.o
	g++ -std=c++11 regex.o value.o token.o main.o -o armin

main.o: main.cpp
	g++ -std=c++11 -g -c  main.cpp

token.o: token.cpp
	g++ -std=c++11 -g -c token.cpp 

value.o: value.cpp
	g++ -std=c++11 -g -c value.cpp 

regex.o: regex.cpp 
	g++ -std=c++11 -g -c regex.cpp

clean:
	rm *.o armin
