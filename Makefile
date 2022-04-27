prog : PatternSearchClient.o aho_corasick.o LAT.o LAM.o DIPWM.o SDSL.o
	g++ -Wall  -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib PatternSearchClient.o  LAT.o LAM.o DIPWM.o SDSL.o -lpthread -lsdsl -ldivsufsort -ldivsufsort64 -o prog
SDSL.o : PatternSearchLib/SDSL.cpp
	g++ -Wall -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib PatternSearchLib/SDSL.cpp -c -lsdsl -ldivsufsort -ldivsufsort64
DIPWM.o : PatternSearchLib/DIPWM.cpp
	g++ -c -Wall PatternSearchLib/DIPWM.cpp
LAM.o : PatternSearchLib/LAM.cpp	
	g++ -c -Wall PatternSearchLib/LAM.cpp
LAT.o : PatternSearchLib/LAT.cpp
	g++ -c -Wall PatternSearchLib/LAT.cpp
aho_corasick.o : PatternSearchLib/aho_corasick.hpp
	g++ -c -Wall PatternSearchLib/aho_corasick.hpp
PatternSearchClient.o : PatternSearchClient/PatternSearchClient.cpp	
	g++ -c -Wall PatternSearchClient/PatternSearchClient.cpp

