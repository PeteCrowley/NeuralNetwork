# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++14 -g -O3 -Wall -Xpreprocessor -fopenmp

# Include directories
INCLUDES = -Iinclude -I/usr/local/include -I/usr/local/opt/libomp/include

# Library directories and flags
LDFLAGS = -L/usr/local/lib -L/usr/local/opt/libomp/lib

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
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) -lsfml-graphics -lsfml-window -lsfml-system -lomp

$(TARGET2): $(MNIST_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) -lomp

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(VISUAL_OBJS) $(MNIST_OBJS) $(TARGET1) $(TARGET2)

# Phony targets
.PHONY: all clean
