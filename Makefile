CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -fmodules-ts 
LD=g++
LDFLAGS=-std=c++20 -fmodules-ts -lCatch2

OBJECTS=memory.o emulator.o
TEST_OBJECTS=$(OBJECTS) tests/memory.o tests/main.o

all: gcm.cache/mark $(OBJECTS) 
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp) -o $@
unittest: $(TEST_OBJECTS)
	$(LD) $(LDFLAGS) $(TEST_OBJECTS) -o unittest
test: gcm.cache/mark unittest
	./unittest
gcm.cache/mark:
	$(CXX) $(CXXFLAGS) -x c++-system-header iostream
	$(CXX) $(CXXFLAGS) -x c++-system-header map
	$(CXX) $(CXXFLAGS) -x c++-system-header vector
	$(CXX) $(CXXFLAGS) -x c++-system-header cstdint
	$(CXX) $(CXXFLAGS) -x c++-system-header cstddef
	touch gcm.cache/mark
clean:
	rm -rf gcm.cache
	find . -name '*.o' -exec rm {} \;
