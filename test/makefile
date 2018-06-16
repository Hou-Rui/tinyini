target = test
objects = tinyini.o test.o
cxxflags = -std=c++11
cc = g++

$(target): $(objects)
	$(cc) $(cxxflags) $(objects) -o $(target)
	rm $(objects)
tinyini.o: ../src/tinyini.hpp ../src/tinyini.cpp
	$(cc) -c $(cxxflags) ../src/tinyini.cpp -o tinyini.o
test.o: test.cpp
	$(cc) -c $(cxxflags) test.cpp -o test.o

.PHONY:clean
clean:
	rm -rf $(objects) $(target)