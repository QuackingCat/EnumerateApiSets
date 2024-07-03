#include <stdio.h>

int main()
{
	printf("there are %d implemented api sets on this machine.\n", parser("apisets.csv"));
}
