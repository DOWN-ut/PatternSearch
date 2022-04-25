BIN=bin/client

# liste des fichiers sources 
DIPWN=PatternSearchLib/DIPWM.cpp
LAM=PatternSearchLib/LAM.cpp
LAT=PatternSearchLib/LAT.cpp
AHO=PatternSearchLib/aho_corasick.hpp
CLIENT=PatternSearchClient/PatternSearchClient.cpp

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@

bin/client: $(CLIENT:%.c=obj/%.o) $(DIPWN:%.c=obj/%.o) $(LAM:%.c=obj/%.o) $(LAT:%.c=obj/%.o) $(AHO:%.c=obj/%.o)
	g++ -o $@ $+
	
clean:
	rm -f $(BIN) obj/*.o *~
