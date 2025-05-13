CFLAGS = -g -O0 -ansi -pedantic -Wall -Wextra -std=c++20

all:
	flex al.l
	bison --report=all -Wcounterexamples parser.y
	g++ $(CFLAGS) -o al al.c help_functions.cpp symbol_table.cpp handlers.cpp icode_managers.cpp tcode_generators.cpp parser.c

clean:
	rm *.o