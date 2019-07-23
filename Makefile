all: test.o run

run: 
	./main af_0_k101.mtx b_sparse_af_0_k101.mtx

test.o: test.cpp SparseMatrix.o Solver.o
	g++ -Wall -o test.o -fopenmp -c test.cpp
	g++ -o main test.o SparseMatrix.o Solver.o -fopenmp

Solver.o: Solver.cpp SparseMatrix.h
	g++ -Wall -o Solver.o -fopenmp -O3 -c Solver.cpp

Solver.cpp: generator2.py
	python3.6 generator2.py

SparseMatrix.o: SparseMatrix.cpp SparseMatrix.h
	g++ -Wall -o SparseMatrix.o -fopenmp -O3 -c SparseMatrix.cpp

clean: 
	rm main