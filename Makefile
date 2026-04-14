# Compiler
CXX = /usr/bin/clang++

# Compiler flags
CXXFLAGS = -g -O2 -Wall -std=c++17 -Xpreprocessor -fopenmp

# Include directories
INCLUDES = -I/opt/homebrew/opt/libomp/include -I/opt/homebrew/cellar/sfml/3.0.2/include -I./include

# Library directories and flags
LDFLAGS = -L/opt/homebrew/opt/libomp/lib -L/opt/homebrew/cellar/sfml/3.0.2/lib
OMP_LIBS = -lomp

# Source files
SRCS_VISUAL = src/ActivationFunction.cpp src/CostFunction.cpp src/EvaluationFunctions.cpp src/Layer.cpp src/Network.cpp src/VisualizeClassification.cpp
SRCS_MNIST = src/ActivationFunction.cpp src/CostFunction.cpp src/EvaluationFunctions.cpp src/Layer.cpp src/Network.cpp src/Mnist.cpp src/CSVReader.cpp

# Object files
VISUAL_OBJS = $(SRCS_VISUAL:.cpp=.o)
MNIST_OBJS = $(SRCS_MNIST:.cpp=.o)

# Executable name
TARGET1 = VisualizeClassification

TARGET2 = Mnist

# Default target
all: $(TARGET1) $(TARGET2)

# Link the executable
$(TARGET1): $(VISUAL_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) -lsfml-graphics -lsfml-window -lsfml-system $(OMP_LIBS)

$(TARGET2): $(MNIST_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(OMP_LIBS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(VISUAL_OBJS) $(MNIST_OBJS) $(TARGET1) $(TARGET2)

# Phony targets
.PHONY: all clean
