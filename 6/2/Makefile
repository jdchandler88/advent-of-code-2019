.PHONY : unity clean all

clean :
	rm -f exe test

all : clean exe test

unity : libunity.a

libunity.a :
	cd ../../Unity/src && cmake
	cd ../../Unity/src && make
 
exe : unity
	gcc -o exe -I../../utils -I. ../../utils/utils.c sol.c main.c  

test : unity test.c
	gcc -o test -I../../Unity/src -I../../utils sol.c test.c ../../utils/utils.c  -L../../Unity/src -lunity -DTEST 