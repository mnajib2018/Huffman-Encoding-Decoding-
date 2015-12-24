all: hencode hdecode

hencode: hencode.o
	g++ -o hencode hencode.o

hencode.o: hencode.cc
		g++ -c hencode.cc

hdecode: hdecode.o
	g++ -o hdecode hdecode.o

hdecode.o: hdecode.cc
	g++ -c hdecode.cc
