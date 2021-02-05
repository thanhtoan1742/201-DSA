run_out: run_minPark_out run_busParkin_out

run_ans: run_minPark_ans run_busParkin_ans

run: run_minPark run_busParking

run_minPark_ans: build_minPark
	./minPark.exe > minPark.ans

run_busParkin_ans: build_busParking
	./busParking.exe > busParking.ans

run_minPark_out: build_minPark
	./minPark.exe > minPark.out

run_busParkin_out: build_busParking
	./busParking.exe > busParking.out

run_minPark: build_minPark
	./minPark.exe

run_busParking: build_busParking
	./busParking.exe

build_minPark: minPark.cpp
	g++ -std=c++11 -Wall minPark.cpp -o minPark.exe

build_busParking: busParking.cpp
	g++ -std=c++11 -Wall busParking.cpp -o busParking.exe