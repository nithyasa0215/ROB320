CXX = g++
CXXFLAGS = -std=c++17
PROGRAMS = simple_parser robot_parser

all: $(PROGRAMS)

simple_parser: simple_parser.cpp
	$(CXX) $(CXXFLAGS) -o simple_parser simple_parser.cpp

robot_parser: robot_parser.cpp
	$(CXX) $(CXXFLAGS) -o robot_parser -I Eigen/ robot_parser.cpp

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean