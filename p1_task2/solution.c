#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <stddef.h> // defines NULL

typedef int (*FT)(int);

FT init_lib(const char *lib, const char* fun)
{
	void *hdl = dlopen(lib, RTLD_LAZY);
	if(!hdl)
		return NULL;
	return (FT)dlsym(hdl, fun);
}

int main(int argc, char* argv[])
{
	if(argc < 4)
		return 1;
	FT f = init_lib(argv[1], argv[2]);
	if(!f)
		return 1;
	
	printf("%d\n", f(atoi(argv[3])));
		
	return 0;
}
