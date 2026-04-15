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
SRCS_DIGIT = src/ActivationFunction.cpp src/CostFunction.cpp src/Layer.cpp src/Network.cpp src/CSVReader.cpp src/DigitVisualizer.cpp
SRCS_DRAWER = src/ActivationFunction.cpp src/CostFunction.cpp src/Layer.cpp src/Network.cpp src/DigitDrawer.cpp

# Object files
VISUAL_OBJS = $(SRCS_VISUAL:.cpp=.o)
MNIST_OBJS = $(SRCS_MNIST:.cpp=.o)
DIGIT_OBJS = $(SRCS_DIGIT:.cpp=.o)
DRAWER_OBJS = $(SRCS_DRAWER:.cpp=.o)

# Executable name
TARGET1 = VisualizeClassification

TARGET2 = Mnist

TARGET3 = DigitVisualizer

TARGET4 = DigitDrawer

# Default target
all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)

# Link the executable
$(TARGET1): $(VISUAL_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) -lsfml-graphics -lsfml-window -lsfml-system $(OMP_LIBS)

$(TARGET2): $(MNIST_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(OMP_LIBS)

$(TARGET3): $(DIGIT_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) -lsfml-graphics -lsfml-window -lsfml-system $(OMP_LIBS)

$(TARGET4): $(DRAWER_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) -lsfml-graphics -lsfml-window -lsfml-system $(OMP_LIBS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(VISUAL_OBJS) $(MNIST_OBJS) $(DIGIT_OBJS) $(DRAWER_OBJS) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)

# Phony targets
.PHONY: all clean
