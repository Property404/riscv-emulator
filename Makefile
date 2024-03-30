CXX=g++
CXXFLAGS=-std=c++20 -fmodules-ts -Wall
LD=g++
LDFLAGS=-std=c++20 -fmodules-ts -lCatch2

all: build_system_modules  test/test
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp) -o $@
test/test: test/main.o
	$(LD) $(LDFLAGS) test/main.o -o test/test
build_system_modules:
	$(CXX) $(CXXFLAGS) -xc++-system-header iostream
clean:
	rm -rf gc.cache
	rm $(find . -name *.o)


