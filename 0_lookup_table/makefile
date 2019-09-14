all: lookup

lookup: cpp/*
	g++ -g -fsanitize=address -o lookup.out cpp/* -std=c++11 -Ihpp -Iimports

