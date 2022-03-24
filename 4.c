#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		perror("command file N\n file - file to read\n N - number of chars to read");
		return -1;
	}
	char *endptr;
	long int N = strtol(argv[2], &endptr, 10);
	if (errno == ERANGE)
	{
		perror("N (number of chars to read) is out of range");
		return -1;
	}
	if (endptr[0] == argv[2][0])
	{
		perror("invalid: N - number of chars to read");
		return -1;
	}
	endptr = NULL;
	if (N < 0)
	{
		perror("N < 0. If you wish to print all text input N = 0");
		return -1;
	}
	
	FILE *f;
	if (!(f = fopen(argv[1], "r") ))
	{
		perror("error 1 : Cannot open file");
		return 1;
	}

	int start = (N == 0 ? 1 : 0);
	int c;
	int flag = 1;

	while (flag)
	{
		int i = start;
		while (i != N)
		{
			if (EOF != (c = getc(f)))
			{
				if (c == '\n') 
					i++;
			
				if (EOF == putc(c, stdout))
				{
					perror("error 2: Cannot write char to Terminal");
					return 2;
				}
			}
			else
			{
				flag = 0;
				break;
			}

		}
		getc(stdin);

	}

	if (fclose(f))
	{
		perror("error 3 : Cannot close file");
		return 3;
	}

	return 0;
}
