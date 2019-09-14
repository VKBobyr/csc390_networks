all: lookup

lookup: cpp/*
	g++ -g -c -ggdb -fsanitize=address -fno-omit-frame-pointer cpp/* -std=c++11 -Ihpp
cleanup:
	rm ./*.o