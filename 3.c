#include <stdio.h>

#define END_MARK 6
int main(int argc, char *argv[])
{
	if (argc != 2) 
	{
		fprintf(stderr, "command file\n file - file to write from terminal in");
		return -1;
	}
	FILE *f;
	if (!(f = fopen(argv[1], "w") ))
	{
		perror("error 1 : Cannot open file");
		return 1;
	}

	int c;
	while (EOF != (c = getc(stdin)) && c != END_MARK)
	{
		if (EOF == putc(c, f))
		{
			perror("error 2: Cannot write into file");
			return 2;
		}
	}

	if (fclose(f))
	{
		perror("error 3 : Cannot close file");
		return 3;
	}


	return 0;
}
