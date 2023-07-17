all: exe

exe: main.c hello.h lib
	gcc main.c -o hello_exe -L. -lHello

lib: hello.c hello.h
	gcc -o libHello.so -shared hello.c

clean:
	-rm libHello.so hello_exe
