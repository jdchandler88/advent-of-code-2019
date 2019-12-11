.PHONY : unity clean all

clean :
	rm -f exe

all : clean exe

unity : libunity.a

libunity.a :
	cd ../../Unity/src && cmake
	cd ../../Unity/src && make
 
exe : unity
	gcc -o exe -I../../utils -I../1 ../../utils/utils.c ../1/sol.c main.c  