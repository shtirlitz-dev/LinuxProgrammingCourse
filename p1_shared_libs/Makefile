all: exe

exe: main.c lib
	gcc main.c -o hello_exe -L. -lHello

lib: hello.c
	gcc -o libHello.so -shared hello.c

hello.c: hello.h

main.c: hello.h

clean:
	-rm libHello.so hello_exe
