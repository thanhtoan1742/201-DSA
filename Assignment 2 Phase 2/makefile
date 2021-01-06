run: run_sub1 run_sub2

run_sub1: build_sub1
	./sub1

run_sub2: build_sub2
	./sub2

build_sub1: sub1.cpp
	g++ -std=c++11 -Wall sub1.cpp -o sub1

build_sub2: sub2.cpp
	g++ -std=c++11 -Wall sub2.cpp -o sub2