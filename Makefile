BIN=bin/exo1  bin/rdv	bin/traitementImage

# liste des fichiers sources 
DIPWN=../PatternSearchLib/DIPWM.cpp
LAM=../PatternSearchLib/LAM.cpp
LAT=../PatternSearchLib/LAT.cpp
aho_corasick=../PatternSearchLib/aho_corasick.hpp
DIR_GUARD=@mkdir -p $(@D)
CC=gcc 
CFLAGS=-Wall -Iinclude

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@

bin/exo1: $(EX1:%.c=obj/%.o)
	$(DIR_GUARD)
	$(CC) -o $@ $+ obj/calculC.o 

bin/rdv: $(RDV:%.c=obj/%.o)
	$(DIR_GUARD)
	$(CC) -o $@ $+ obj/calculC.o 

bin/traitementImage: $(IMG:%.c=obj/%.o)
	$(DIR_GUARD)
	$(CC) -o $@ $+ obj/calculC.o 
	
clean:
	rm -f $(BIN) obj/*.o *~