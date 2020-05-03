FLAGS=--std=c++11 -g -Wall

all: simul

simul: main.o
	g++ $(FLAGS) -o simul main.o
	
main.o: main.cpp
	g++ $(FLAGS) -c main.cpp

clean:
	rm -rf *.o
