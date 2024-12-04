CXX = g++
CXXFLAGS = -std=c++11 -Wall `fltk-config --cxxflags`
LDFLAGS = `fltk-config --ldflags`
TARGET = OOP_HW2

all: $(TARGET)

$(TARGET): OOP_HW2.cpp
	$(CXX) $(CXXFLAGS) OOP_HW2.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
