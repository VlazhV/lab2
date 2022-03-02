#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>

int main()
{
	DIR *dir;
	if (! (dir = opendir(".")))
	{
		perror("error 1:1 : cannot open dir");
		return 1;
	}

	struct dirent *dire;

	while (dire = readdir(dir))
		puts(dire->d_name);

	if (closedir(dir))
	{
		perror("error 2:1 : cannot close dir");
		return 2;
	}
	puts("################");

	if (! (dir = opendir("/")))
	{
		perror("error 1:2 : cannot open dir");
		return 1;
	}

	while (dire = readdir(dir))
		puts(dire->d_name);

	if (closedir(dir))
	{
		perror("error 2:2 : cannot close dir");
		return 2;
	}

	return 0;
}
