#include <stdbool.h>
#include <dlfcn.h>

// #include <stddef.h> // defines NULL

void (*hello_message)(const char *);

bool init_lib()
{
	void *hdl = dlopen("libHello.so", RTLD_LAZY);
	if(!hdl)
		return false;
	hello_message = (void (*)(const char *))dlsym(hdl, "hello_message");
	return true;
}


int main()
{
	if(init_lib())
		hello_message("me");
		
	return 0;
}
