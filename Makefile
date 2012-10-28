
klondike: klondike.o shuffle.o
	g++ -o klondike klondike.o shuffle.o

klondike.o: klondike.cpp klondike.hh shuffle.hh
shuffle.o: shuffle.cpp shuffle.hh

