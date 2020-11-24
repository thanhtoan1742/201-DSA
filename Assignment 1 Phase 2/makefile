unit: unitExe
	./unit 

unitExe: unitTest.cpp bus.hpp
	g++ -std=c++11 -Wall unitTest.cpp -o unit

custom: customExe
	./custom

customExe: customTest.cpp bus.hpp
	g++ -std=c++11 -Wall customTest.cpp -o custom
	
CLI: CLIExe
	./CLI

CLIExe: CLI.cpp bus.hpp
	g++ -std=c++11 -Wall CLI.cpp -o CLI
