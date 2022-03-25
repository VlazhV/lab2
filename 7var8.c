#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>


long long int countSize(char *path);
char *getAbsPath(char *relPath);

void formatOut(long long int size);

int main(int argc, char *argv[])
{
	
	
	char *absPath = (char *)calloc(PATH_MAX, 1);
	if (!(absPath = getAbsPath(argv[1])))
	{
		perror("error m1 : cannot get absPath");
		return 1;
	} 
	
	long long int size = countSize(absPath);
	
	formatOut(size);

		
	return 0;
}
char *getAbsPath(char *relPath)
{
	if (relPath[0] == '/')
		return relPath;
	else
	{
		char curDir[PATH_MAX];
		if (!getcwd(curDir, PATH_MAX))
		{
			perror("error gap1 : getcwd() failed");
			return NULL;	
		}

		return strcat(strcat(curDir, "/"), relPath);
	}
	
}

long long int countSize(char *path)
{
	long int size;
	
	DIR* curDir;
	if (!(curDir = opendir(path)))
	{
		fprintf(stderr, "error cs1: opendir() failed - %s\n", path);
		return 1;
	}
	
	struct dirent *dire;
	char *newPath = (char *)calloc(PATH_MAX, 1);
	while (dire = readdir(curDir))
	{
		if ( strcmp(dire->d_name, ".") == 0 || strcmp(dire->d_name, "..") == 0 )
			continue;


		strcpy(newPath, path);
		strcat(strcat(newPath, "/"), dire->d_name);
		puts(newPath);
		if (dire->d_type == DT_DIR)
		{	
			int addSize = countSize(newPath);	
			if (addSize < 0)
			{
				puts("zero less");
				sleep(5);
			}
			size += addSize;						
		}
		else
		{		
			struct stat fileStat;
			if (lstat(newPath, &fileStat))
			{
				fprintf(stderr, "error cs2 : lstat() failed - %s\n", newPath);
				continue;
			}
			printf("size = %lu B\n\n", fileStat.st_size);
			size += fileStat.st_size;
		}	
			
	
	}
	
	if (closedir(curDir))
	{
		fprintf(stderr, "error cs3: closedir() failed - %s\n", path);
		return -1;
	}
	
	
	
	return size;	
}

void formatOut(long long int size)
{
	if (size > (1<< 29))
		printf("size - %f GB\n", (float)size/(1<<30));
	else if (size > (1 << 19))
		printf("size - %f MB\n", (float)size/(1<<20));
	else if (size > (1 << 9))
		printf("size - %f kB\n", (float)size/(1<<10));
	else
		printf("size - %lld B\n", size);
		
	return;
}
/*
	struct statfs statFs;
	if (statfs("/", &statFs))
	{
		perror("error m2: cannot get stats of root dir");
		return 2;
	}
	
	printf("total blocks in fs = %lu blocks\n", statFs.f_blocks);
	printf("free blocks in fs = %lu blocks\n", statFs.f_bfree);
	printf("total size of fs = %f GB\n", (float)statFs.f_bfree * statFs.f_bsize/(1<<30));
*/
