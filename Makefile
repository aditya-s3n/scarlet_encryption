# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g -I./inc

# Directories
SRC_DIR = ./src
BIN_DIR = ./bin

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
EXEC = ./scarlet

# Targets
all: $(EXEC)

$(EXEC): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -g -o $@

clean:
	rm -f $(EXEC)