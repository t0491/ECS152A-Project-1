FLAGS=--std=c++11 -g -Wall

all: simulation

simulation: main.o
	g++ $(FLAGS) -o simulation main.o
	
main.o: main.cpp
	g++ $(FLAGS) -c main.cpp

clean:
	rm -rf *.o
