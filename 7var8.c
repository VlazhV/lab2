#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>


struct _size_
{
	long long int size;
	long long int diskUse;
};

struct _size_ countSize(char *path);
char *getAbsPath(char *relPath);
void formatOut(long long int size, const char *typeSize);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		perror("command dir");
		return -1;
	}
	
	char *absPath = (char*)calloc(PATH_MAX, 1);
	if (!(absPath = getAbsPath(argv[1])))
	{
		perror("error m1 : cannot get absPath");
		return 1;
	} 
	
	struct _size_ sizeDu = countSize(absPath);

	
	formatOut(sizeDu.size, "size");
	formatOut(sizeDu.diskUse, "disk use");
	
	struct statfs statFs;
	if (statfs(absPath, &statFs))
	{
		perror("error m2: cannot get stats of root dir");
		return 2;
	}
	
	
	long long unsigned int disk = statFs.f_blocks * statFs.f_bsize;
	printf("disk size = %f GB\n", (float)disk/(1<<30));

	printf("%f%%\n", (float)100*sizeDu.diskUse/disk);
		
	
	return 0;
}
char* getAbsPath(char *relPath)
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

struct _size_ countSize(char *path)
{
	struct _size_ sizeDu = {0, 0};
	const struct _size_ exitError = {-1, -1};
	
	DIR* curDir;
	if (!(curDir = opendir(path)))
	{
		fprintf(stderr, "error cs1: opendir() failed - %s ", path);
		perror("");
		return exitError;
	}
	
	
	struct dirent *dire;
	char *newPath = (char*)calloc(PATH_MAX, 1);
	while (dire = readdir(curDir))
	{
		if ( strcmp(dire->d_name, ".") == 0 || strcmp(dire->d_name, "..") == 0 )
			continue;


		strcpy(newPath, path);
		strcat(strcat(newPath, "/"), dire->d_name);
		if (dire->d_type == DT_DIR)
		{	
			struct _size_ addSize = countSize(newPath);	
			
			sizeDu.size += addSize.size;
			sizeDu.diskUse += addSize.diskUse;			
		}
		else 
		{					
			puts(newPath);
			struct stat fileStat;
			if (lstat(newPath, &fileStat))
			{
				fprintf(stderr, "error cs2 : lstat() failed - %s\n", newPath);
				continue;
			}
			printf("size = %lu B\t", fileStat.st_size);
			printf("disk Use = %lu B\n\n", fileStat.st_blocks * fileStat.st_blksize);
			sizeDu.size += fileStat.st_size;
			sizeDu.diskUse += fileStat.st_blocks * 512;
		}	
	}
	free(newPath);
	if (closedir(curDir))
	{
		fprintf(stderr, "error cs3: closedir() failed - %s ", path);
		perror("");
		return exitError;
	}
	
	
	
	return sizeDu;	
}

void formatOut(long long int size, const char *sizeType)
{
	if (size > (1<< 29))
		printf("%s - %f GB\n", sizeType, (float)size/(1<<30));
	else if (size > (1 << 19))
		printf("%s - %f MB\n", sizeType, (float)size/(1<<20));
	else if (size > (1 << 9))
		printf("%s - %f kB\n", sizeType, (float)size/(1<<10));
	else
		printf("%s - %lld B\n", sizeType, size);
		
	return;
}


