CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -I.
TARGET   := main

# Find all .cpp files in the current folder, src/, and ProcessCollector/
SRCS := $(shell find . -name '*.cpp')
OBJS := $(SRCS:.cpp=.o)
ASMS := $(SRCS:.cpp=.s)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.s: %.cpp
	$(CXX) $(CXXFLAGS) -S -masm=intel -fverbose-asm $< -o $@

asm: $(ASMS)

clean:
	rm -f $(OBJS) $(ASMS) $(TARGET)

.PHONY: all clean asm