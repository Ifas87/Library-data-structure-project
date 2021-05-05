CC = g++ 

.PHONY: clean remove

all: run clean

testing: testcase clear

run: output
	./output $(filename)

output: CW2.o classes.o
	$(CC) CW2.o classes.o -o output

testcase: tests
	./tests

tests: tests.o classes.o
	$(CC) tests.o classes.o -o tests

tests.o: tests.cpp
	$(CC) -c tests.cpp

CW2.o: CW2.cpp
	$(CC) -c CW2.cpp

classes.o: classes.cpp classes.h
	$(CC) -c classes.cpp

clean:
	rm *.o output

clear:
	rm *.o tests