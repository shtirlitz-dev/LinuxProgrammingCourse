#include <stdio.h>
#include <string.h>

int stringStat(const char *string, size_t multiplier, int *count)
{
	++*count;
	return string? strlen(string) * multiplier : 0;
}
