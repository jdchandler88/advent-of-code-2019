.PHONY : unity clean all

clean :
	rm -f exe test

all : clean exe test

unity : libunity.a

libunity.a :
	cmake ../../Unity/src/CmakeLists.txt
	make ../../Unity/src/Makefile

exe : unity
	gcc -o exe -I../../utils -I. ../../utils/utils.c password.c main.c  

test : unity test.c
	gcc -o test -I../../Unity/src -I../../utils password.c test.c ../../utils/utils.c  -L../../Unity/src -lunity -DTEST 