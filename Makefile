BIN=bin/client

# liste des fichiers sources 
DIPWN=PatternSearchLib/DIPWM.cpp
LAM=PatternSearchLib/LAM.cpp
LAT=PatternSearchLib/LAT.cpp
AHO=PatternSearchLib/aho_corasick.hpp
CLIENT=PatternSearchClient/PatternSearchClient.cpp
FM_INDEX = PatternSearchClient/sdsl/fm_index.o
default: $(BIN)

subsystem:
	cd PatternSearchLib/sdsl/Makefile && $(MAKE)


########################################
#~ regles pour l'executable
########################################

obj/%.o: %.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@

bin/client: $(CLIENT:%.c=obj/%.o) $(DIPWN:%.c=obj/%.o) $(LAM:%.c=obj/%.o) $(LAT:%.c=obj/%.o) $(AHO:%.c=obj/%.o) $(FM_INDEX:%.o=obj/%.o)
	g++ -o $@ $+ -lpthread
	
clean:
	rm -f $(BIN) obj/*.o *~
