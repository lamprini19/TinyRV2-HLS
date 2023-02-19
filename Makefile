include_path = include
output_path  = bin
executable   = main

.PHONY: all
all: $(output_path)/$(executable)

$(output_path)/$(executable): src/main.cpp bin/Processor.o bin/ALU.o
	g++ -o bin/main -I $(include_path) src/main.cpp bin/Processor.o bin/ALU.o

bin/Processor.o: src/Processor.cpp $(include_path)/Processor.h $(include_path)/Operation.h
	g++ -c -o bin/Processor.o -I $(include_path) src/Processor.cpp

bin/ALU.o:  src/ALU.cpp $(include_path)/ALU.h
	g++ -c -o bin/ALU.o -I $(include_path) src/ALU.cpp

.PHONY: clean
clean:
	rm bin/*
