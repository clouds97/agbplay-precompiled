CXX = clang++
CXXFLAGS = -Wall -Wextra -Wconversion -std=c++0x -O3
BINARY = agbplay
LIBS = -L. -l:libportaudio.so -lm -lncursesw -lboost_system -lboost_thread
IMPORT = -I ../portaudio/include

SRC_FILES = $(wildcard *.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

.PHONY: all
all: $(BINARY)

.PHONY: clean
	rm -f $(OBJ_FILES)

$(BINARY): $(OBJ_FILES)
	$(CXX) -o $@ $(CXXFLAGS) $(LIBS) $^

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(IMPORT)
