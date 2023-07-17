#include <stdio.h>

int stringStat(const char *string, size_t multiplier, int *count);

int main()
{
	int cnt = 0;
	int a = stringStat("", 2, &cnt);
	int b = stringStat(0, 3, &cnt);
	int c = stringStat("my str", 5, &cnt);
	printf("%d %d %d %d", a, b, c, cnt);
	return 0;
}
