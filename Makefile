DEST     =  dest
CXX      =  g++
CXXFLAGS =  -std=c++20 -Wall

ifdef DEBUG
CXXFLAGS += -g
else
CXXFLAGS += -O3
endif

LIB_O=$(shell gfind lib -name '*.cpp' -printf './dest/%f.o ' | sed 's/.cpp//g')

build: prepare $(LIB_O) colorxf

prepare:
	mkdir -p $(DEST)

./$(DEST)/%.o: lib/%.cpp lib/%.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

colorxf: main.cpp $(LIB_O)
	$(CXX) $(CXXFLAGS) main.cpp -o colorxf $(LIB_O)

clean:
	rm -rf $(DEST) colorxf
