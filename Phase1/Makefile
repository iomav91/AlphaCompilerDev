CFLAGS = -ansi -pedantic -Wall -Wextra -std=c++11

all:
	flex al.l
	g++ $(CFLAGS) -o al al.c help_functions.cpp

clean:
	rm *.o