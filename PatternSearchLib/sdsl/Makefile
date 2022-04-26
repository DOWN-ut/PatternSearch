include ../sdsl-source/Make.helper
CXX_FLAGS=$(MY_CXX_FLAGS) $(MY_CXX_OPT_FLAGS) -I$(INC_DIR) -L$(LIB_DIR) 
CCLIB=-lsdsl -ldivsufsort -ldivsufsort64 
SOURCES=$(wildcard *.cpp)
EXECS=$(SOURCES:.cpp=.o)

ALL: $(EXECS)

build-test: $(EXECS)
	        
%.o:%.cpp 
	$(MY_CXX) $(CXX_FLAGS) -o $@ $< $(CCLIB) 

clean:
	rm -f $(EXECS)
	rm -rf *.dSYM
