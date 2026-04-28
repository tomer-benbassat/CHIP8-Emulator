CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g -Iinclude

LDFLAGS = `sdl2-config --cflags --libs`

SOURCES = main.cpp \
		  src/chip8.cpp \
		  src/chip8_opcodes.cpp \
		  src/display.cpp \
		  src/platform.cpp

TARGET = chip8

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)