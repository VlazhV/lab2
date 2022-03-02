#include <stdio.h>

int main()
{
	char Msg[] = "Hello, World";
	for (int i = 0; Msg[i] != '\0'; ++i)
		putc(Msg[i], stdout);

	return 0;
}
