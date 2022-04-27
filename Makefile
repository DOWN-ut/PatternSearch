BIN=bin/client

# liste des fichiers sources 
DIPWN=PatternSearchLib/DIPWM.cpp
LAM=PatternSearchLib/LAM.cpp
LAT=PatternSearchLib/LAT.cpp
AHO=PatternSearchLib/aho_corasick.hpp
CLIENT=PatternSearchClient/PatternSearchClient.cpp
SDSL=PatternSearchLib/SDSL.cpp

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@

SDSL.o: PatternSearchLib/SDSL.cpp
	g++ -Wall -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib PatternSearchLib/SDSL.cpp -c -lsdsl -ldivsufsort -ldivsufsort64

bin/client: $(CLIENT:%.c=obj/%.o) $(DIPWN:%.c=obj/%.o) $(LAM:%.c=obj/%.o) $(LAT:%.c=obj/%.o) $(AHO:%.c=obj/%.o) SDSL.o
	g++ -Wall -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib  -o $@ $+ -lpthread -lsdsl -ldivsufsort -ldivsufsort64
	
clean:
	rm -f $(BIN) obj/*.o *~
