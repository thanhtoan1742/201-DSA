run: build
	./main

build: main.cpp BKUTree.hpp
	g++ -std=c++11 -Wall main.cpp -o main

unit_test: build_unit_test
	./unit_test

build_unit_test: unit_test.cpp BKUTree.hpp BKUTree.cpp 
	g++ -std=c++11 -Wall unit_test.cpp -o unit_test
