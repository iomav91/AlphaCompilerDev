CFLAGS = -ansi -pedantic -Wall -Wextra -std=c++11

all:
	flex al.l
	bison --report=all -Wcounterexamples parser.y
	g++ $(CFLAGS) -o al al.c help_functions.cpp symbol_table.cpp handlers.cpp parser.c

clean:
	rm *.o