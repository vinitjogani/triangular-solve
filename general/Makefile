CFLAGS=-Wall -fopenmp -std=c++11 -O3
HEADERS=includes.h analysis/Graph.h matrix/Sparse.h reader/Reader.h reader/MTXReader.h

main: main.o matrix/Sparse.o analysis/Graph.o reader/MTXReader.o reader/Reader.o matrix/Utils.o
	g++ -o main *.o ${CFLAGS}

%.o: %.cpp ${HEADERS}
	g++ -c $< ${CFLAGS}

clean:
	rm *.o
	rm main