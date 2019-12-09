.PHONY : unity clean all

clean :
	rm -f exe

all : clean exe

unity : libunity.a

libunity.a :
	cmake ../../Unity/src/CmakeLists.txt
	make ../../Unity/src/Makefile

exe : unity
	gcc -o exe -I../../utils -I../1 ../../utils/utils.c ../1/intcode.c main.c  