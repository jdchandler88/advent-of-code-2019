.PHONY : unity clean all

clean :
	rm test exe

all : clean exe test

unity : libunity.a

libunity.a :
	cmake ../Unity/src/CmakeLists.txt
	make ../Unity/src/Makefile

exe : unity
	gcc -o mass -E fuelCalculator.c

test : unity utils.c
	gcc -o test testUtils.c utils.c -I../Unity/src -L../Unity/src -lunity