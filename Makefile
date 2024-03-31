CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -fmodules-ts 

OBJECTS=test/harness.o memory.o instruction.o emulator.o
TEST_OBJECTS=$(OBJECTS) test/assembly.o test/emulator.o test/main.o
MODULES=iostream map vector cstdint cstddef cstdlib variant string functional \
		typeinfo new stdexcept fstream filesystem string_view optional random \
		limits cassert

all: gcm.cache/mark $(OBJECTS) 
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp) -o $@
unittest: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o unittest
.PHONY: test
test: gcm.cache/mark unittest
	./unittest
gcm.cache/mark:
	$(foreach module,$(MODULES),$(CXX) $(CXXFLAGS) -x c++-system-header $(module);)
	touch gcm.cache/mark
refresh:
	rm gcm.cache/mark
clean:
	rm -rf gcm.cache
	find . -name '*.o' -exec rm {} \;
