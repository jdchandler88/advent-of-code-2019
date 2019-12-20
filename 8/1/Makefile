.PHONY : unity clean all

clean :
	rm -f exe test

all : clean exe test

unity : libunity.a

libunity.a :
	cd ../../Unity/src && cmake
	cd ../../Unity/src && make
 
exe : unity
	gcc -o exe -I../../utils main.c sol.c ../../utils/utils.c

test : unity test.c
	gcc -o test -I../../Unity/src -I../../utils sol.c test.c -L../../Unity/src -lunity -DTEST 