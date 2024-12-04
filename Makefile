CXX = g++
CXXFLAGS = -std=c++11 -Wall `fltk-config --cxxflags`
LDFLAGS = `fltk-config --ldflags`
TARGET = fltk-shapedrawer

all: $(TARGET)

$(TARGET): fltk-shapedrawer.cpp
	$(CXX) $(CXXFLAGS) fltk-shapedrawer.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
