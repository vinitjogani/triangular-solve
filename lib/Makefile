all: Reader.o MTXReader.o

MTXReader.o: reader/MTXReader.h reader/MTXReader.cpp
	g++ -c reader/MTXReader.cpp -std=c++11 -Wall

Reader.o: reader/Reader.h reader/Reader.cpp 
	g++ -c reader/Reader.cpp -std=c++11 -Wall

clean:
	rm *.o