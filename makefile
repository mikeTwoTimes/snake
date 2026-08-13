CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -O3 -march=native
INCLUDE = include/snake
TARGET = build/main

all: $(TARGET)

$(TARGET): build build/point.o build/screen.o build/snake.o build/utility.o build/main.o
	g++ $(CXXFLAGS) -lncurses -o $(TARGET) build/point.o build/screen.o build/snake.o build/utility.o build/main.o

build:
	mkdir -p build

build/point.o: $(INCLUDE)/point.h src/point.cpp
	g++ -Iinclude $(CXXFLAGS) -c src/point.cpp -o build/point.o

build/screen.o: $(INCLUDE)/point.h $(INCLUDE)/screen.h src/point.cpp src/screen.cpp
	g++ -Iinclude $(CXXFLAGS) -c src/screen.cpp -o build/screen.o

build/snake.o: $(INCLUDE)/point.h $(INCLUDE)/snake.h src/point.cpp src/snake.cpp
	g++ -Iinclude $(CXXFLAGS) -c src/snake.cpp -o build/snake.o

build/utility.o: $(INCLUDE)/utility.h $(INCLUDE)/screen.h $(INCLUDE)/snake.h src/utility.cpp src/screen.cpp src/snake.cpp
	g++ -Iinclude $(CXXFLAGS) -c src/utility.cpp -o build/utility.o

build/main.o: $(INCLUDE)/utility.h src/utility.cpp app/main.cpp
	g++ -Iinclude $(CXXFLAGS) -c app/main.cpp -o build/main.o

clean:
	rm -rf build
