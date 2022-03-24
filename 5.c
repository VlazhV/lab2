#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		perror("command srcFile destFile");
		return -1;
	}
	FILE *destFile, *srcFile;
	if (! (srcFile = fopen (argv[1], "r") ))
	{
		perror("error 1:1 : Cannot open src file");
		return 1;
	}

	if (! (destFile = fopen(argv[2], "w") ))
	{
		perror("error 1:2 : Cannot open dest file");
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
	if (chmod(argv[2], fileStat.st_mode))
	{
		perror("error 4 : Cannot apply mode changes to file");
		return 4;
	}

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
