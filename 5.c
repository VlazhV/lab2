#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	FILE *destFile, *srcFile;
	if (! (destFile = fopen (argv[1], "r") ))
	{
		perror("error 1:1 : Cannot open file");
		return 1;
	}

	if (! (srcFile = fopen(argv[2], "w") ))
	{
		perror("error 1:2 : Cannot open file");
		return 1;
	}

	int c;

	while (EOF != (c = getc(srcFile)) )
	{
		if (EOF == putc(c, destFile))
		{
			perror("error 2 : Cannot write into file");
			return 2;
		}
	}

	struct stat fileStat;


	if (stat(argv[1], &fileStat))
	{
		perror("error 3 : Cannot get stats of file");
		return 3;
	}
	chmod(argv[2], fileStat.st_mode);

	if (fclose(destFile))
	{
		perror("error 4:1 : Cannot close file");
		return 4;
	}
	if (fclose(srcFile))
	{
		perror("error 4:2 : Cannot close file");
		return 4;
	}

	return 0;
}
