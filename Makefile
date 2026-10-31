CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -O2

evcharging: EVChargingApp.cpp *.h
	$(CXX) $(CXXFLAGS) -o $@ EVChargingApp.cpp

run: evcharging
	./evcharging

test: evcharging
	./tests/smoke.sh

clean:
	rm -f evcharging

.PHONY: run test clean
