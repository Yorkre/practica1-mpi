CXX = mpicxx
CXXFLAGS = -Wall -Wextra -O2

TARGETS = ejercicio1 ejercicio2

all: $(TARGETS)

ejercicio1: ejercicio1.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ejercicio2: ejercicio2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
