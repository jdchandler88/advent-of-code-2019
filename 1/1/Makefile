.PHONY : unity clean all

clean :
	rm test mass

all : clean mass test

unity : libunity.a

libunity.a :
	cmake ../../Unity/src/CmakeLists.txt
	make ../../Unity/src/Makefile

mass : unity
	gcc -o mass fuelCalculator.h

test : unity test.c
	gcc -o test test.c -I../../Unity/src -L../../Unity/src -lunity -DTEST