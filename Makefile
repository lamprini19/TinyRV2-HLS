include_path = include/
# source_path = src/

all: bin/main

bin/main: src/main.cpp bin/Processor.o
	g++ -o bin/main -I $(include_path) src/main.cpp bin/Processor.o

bin/Processor.o: src/Processor.cpp bin/ALU.o
	g++ -c -o bin/Processor.o -I $(include_path) src/Processor.cpp bin/ALU.o

bin/ALU.o:  src/ALU.cpp
	g++ -c -o bin/ALU.o -I $(include_path) src/ALU.cpp

