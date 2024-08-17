# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++14 -g -O3 -Wall -Xpreprocessor -fopenmp

# Include directories
INCLUDES = -Iinclude -I/usr/local/include -I/usr/local/opt/libomp/include

# Library directories and flags
LDFLAGS = -L/usr/local/lib -L/usr/local/opt/libomp/lib

# Source files
SRCS = $(wildcard src/*.cpp)

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = VisualizeClassification

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) -lsfml-graphics -lsfml-window -lsfml-system -lomp

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
