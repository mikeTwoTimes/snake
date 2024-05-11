CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17
SRC = src
TARGET = play

all: $(TARGET)
	rm -f *.o

$(TARGET): main.o Screen.o Random.o Snake.o File_Util.o
	$(CXX) $(CXXFLAGS) -lncurses -o $(TARGET) main.o Screen.o Random.o Snake.o File_Util.o

main.o: $(SRC)/main.cpp $(SRC)/Screen.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/main.cpp

Screen.o: $(SRC)/Screen.cpp $(SRC)/Screen.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/Screen.cpp

Snake.o: $(SRC)/Snake.cpp $(SRC)/Snake.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/Snake.cpp

Random.o: $(SRC)/Random.cpp $(SRC)/Random.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/Random.cpp

File_Util.o: $(SRC)/File_Util.cpp $(SRC)/File_Util.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/File_Util.cpp

clean:
	rm -f $(TARGET)
