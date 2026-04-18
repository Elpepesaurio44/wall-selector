CXX = g++
CXXFLAGS = -std=c++17 $(shell pkg-config --cflags gtkmm-3.0)
LIBS = $(shell pkg-config --libs gtkmm-3.0) -lstdc++fs
TARGET = wall-selector
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
