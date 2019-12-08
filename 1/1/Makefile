.PHONY : unity clean all

clean :
	rm -f mass test

all : clean mass test

unity : libunity.a

libunity.a :
	cmake ../../Unity/src/CmakeLists.txt
	make ../../Unity/src/Makefile

mass : unity
	gcc -o mass -I../../utils fuelCalculator.c ../../utils/utils.c

test : unity test.c
	gcc -o test -I../../Unity/src -I../../utils test.c  -L../../Unity/src -lunity -DTEST 