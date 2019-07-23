CFLAGS=-Wall -fopenmp -std=c++11 -O3
HEADERS=includes.h analysis/Graph.h matrix/Sparse.h reader/Reader.h reader/MTXReader.h

all: Reader.o MTXReader.o Graph.o Sparse.o Main.o main

main: Main.o Sparse.o Graph.o MTXReader.o Reader.o
	g++ -o main Main.o Sparse.o Graph.o MTXReader.o Reader.o ${CFLAGS}

Main.o: Main.cpp 
	g++ -c Main.cpp ${CFLAGS}

Sparse.o: matrix/Sparse.cpp ${HEADERS}
	g++ -c matrix/Sparse.cpp ${CFLAGS}

Graph.o: analysis/Graph.cpp ${HEADERS}
	g++ -c analysis/Graph.cpp ${CFLAGS}

MTXReader.o: reader/MTXReader.cpp ${HEADERS}
	g++ -c reader/MTXReader.cpp ${CFLAGS}

Reader.o: reader/Reader.cpp ${HEADERS}
	g++ -c reader/Reader.cpp ${CFLAGS}

clean:
	rm *.o
	rm main