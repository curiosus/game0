CC=g++ -std=c++11
CFLAGS= -c -Wall
all: go

go: main.o  
	$(CC) main.o -o game0 -lsfml-graphics -lsfml-window -lsfml-system 

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp


clean:
	rm -rf *o game0

