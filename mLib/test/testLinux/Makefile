.SUFFIXES:

CXX = clang++
FLAGS = -g -std=c++11
FLAGS += -I "src"
FLAGS += -I "../../mLib/include"
FLAGS += -I "../../mLib/src"
LFLAGS = -g

SRC = main.cpp mLibSource.cpp
OBJS = $(SRC:.cpp=.o)
EXECUTABLE = mLibTest

.PHONY:	all purge clean

all:	$(EXECUTABLE)

build/%.o:	src/%.cpp
	$(CXX) $(FLAGS) -MP -MD $(<,.o=.d) $< -c -o $@

$(EXECUTABLE):        $(addprefix build/, $(OBJS))
	$(CXX) $^ -o $@ $(LFLAGS)

clean:
	rm -rf build/*.o build/*.d
	rm -rf $(EXECUTABLE)

purge: clean
	rm -rf build/*

# dependency rules
include $(wildcard build/*.d)
