#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
 
 
//   sudo tune2fs -O ^dir_index /dev/sdaXY

 
#define SIZE_BUF 256
 
typedef struct _tFile_
{
	char *path;
	char *name;
	long int size;
} tFile;
 
tFile *fileNameArr;
 
int includeArr(char *fileName, char *path, long int Fsize, tFile *fileNameArr, int *len, int *index);
int sortArr(tFile *fileNameArr, int len, int (*compare) (tFile fileA, tFile fileB));
int lessbyAlpha(tFile fileA, tFile fileB);
int lessbySize(tFile fileA, tFile fileB);
 
 
int Traversal(char *path);
 
int copyFile(char *srcFile, char *destFile);
int EPILOG(tFile *fileNameArr, int len, char *new_dir);
 
 
 
int main(int argc, char *argv[])
//	argv[1] - start dir
//	argv[2] - sort type
//	argv[3] - new dir
{
 

	int len = 100;
	fileNameArr = (tFile *)calloc(len, sizeof(tFile));
	len = Traversal(argv[1]);
	fileNameArr = (tFile*)realloc(fileNameArr, len * sizeof(tFile));
 

	if (argv[2][0] == '2')	
		sortArr(fileNameArr, len, lessbyAlpha);
	else
		sortArr(fileNameArr, len, lessbySize);		
 	
 	
	 	FILE *f;
		if (!(f = fopen("7.txt", "w") ))
		{
			perror("error m2 : cannot open log file");
			return 2;
		}
	 
		for (int i = 0; i < len ; ++i)
			fprintf(f, "%s\t%li\n",fileNameArr[i].name, fileNameArr[i].size);
		fprintf(f, "========\ntotal = %d\n", len);
		
		
		if (fclose(f))
		{
			perror("error m3: cannot close log file");
			return 3;
		}
 
 
	if (EPILOG(fileNameArr, len, argv[3]))
	{
		perror("error m1 : error of copy");
		return 1;
	}
	for (int i = 0; i < len; ++i)
	{
		free(fileNameArr[i].name);
		free(fileNameArr[i].path);
	}
	free(fileNameArr);
	return 0;
}
 
 
int Traversal(char *path)
{
	static int len = 100;
	static int index = 0;
 
	DIR *cur_dir;
 
	cur_dir = opendir(path);
	struct dirent *dire;
 
	while (dire = readdir(cur_dir))
	{
			
		if (strcmp(dire->d_name,".") && strcmp(dire->d_name, ".."))
		{		
			if (dire->d_type != DT_DIR)
			{				
				struct stat fileBuf;
				lstat(dire->d_name, &fileBuf);
				if (includeArr(dire->d_name, path, fileBuf.st_size, fileNameArr, &len, &index))
				{
					perror("Error 1: Cannot include file in list");				
					return -1;
				}
			}
			else
			{						
				char *new_path = (char*)calloc(SIZE_BUF, sizeof(char));
				strcpy(new_path, path);
				strcat(new_path, "/");
				if (Traversal(strcat(new_path, dire->d_name)) < 0)
				{
					perror("Error 2: Unsuccessful traversal"); 
					return -1;
				}
			}
		}
	}
	return index;
}
 
 
 
 
int includeArr(char *fileName, char *path, long int Fsize, tFile *fileNameArr, int *len, int *index)
{
 
	if ((*index) >= (*len))
	{	
		(*len) *= 2;
		if (!(fileNameArr = (tFile *)realloc(fileNameArr, (*len) * sizeof(tFile)) ))
		{
			perror("Error i1: No memory)");
			return 1;
		}
	}
 
 
	if (!( fileNameArr[*index].name = (char*)calloc(strlen(fileName) + 1, sizeof(char)) ))
	{
		perror("Error i1: No memory)");
		return 1;
	}
	if (!( fileNameArr[*index].path = (char *) calloc(SIZE_BUF, sizeof(char)) ))
	{
		perror("Error i1: No memory)");
		return 1;
	}
	fileNameArr[*index].size = Fsize;
	strcpy(fileNameArr[*index].path, path);
	strcpy(fileNameArr[(*index)++].name, fileName);
 
	return 0;
}
 
int sortArr(tFile *fileNameArr, int len, int (*compare) (tFile fileA, tFile fileB))
{
	int i_min;
	tFile tmp;
	for (int i = 0; i < len - 1; ++i)
	{
		i_min = i;
		for (int j = i; j < len; ++j)
			if ( compare(fileNameArr[j], fileNameArr[i_min]) )
				i_min = j;
 
		tmp = fileNameArr[i];
		fileNameArr[i] = fileNameArr[i_min];
		fileNameArr[i_min] = tmp;
	}
 
	return 0;
}
 
int lessbyAlpha(tFile fileA, tFile fileB)
{
	return (strcmp(fileA.name, fileB.name) < 0);
}
int lessbySize(tFile fileA, tFile fileB)
{
	return (fileA.size > fileB.size);
}
 
 
int EPILOG(tFile *fileNameArr, int len, char *new_dir)
{
	mkdir(new_dir, 0777);
	chmod(new_dir, 0777);

 	char *pathNameNew = (char *)calloc(SIZE_BUF, sizeof(char));
 	char *pathNameOld = (char *)calloc(SIZE_BUF, sizeof(char));
 	
 	FILE *fEx, *fNew;

 	
	for (int i = 0; i < len; ++i)
	{ 
		//form new pathName
		strcpy(pathNameNew, new_dir);
	 	strcat(strcat(pathNameNew, "/"), fileNameArr[i].name);
	 	
	 	//form old pathName
		strcpy(pathNameOld, fileNameArr[i].path);
		strcat(strcat(pathNameOld, "/"), fileNameArr[i].name);
		
		if (copyFile(pathNameOld, pathNameNew))
		{
			perror("error E1 : fail at copy");
			return 1;
		}
		
	}
	return 0;
}
 
int copyFile(char *srcFile, char *destFile)
{
	FILE *f_src, *f_dest;
 
	if (!(f_src = fopen(srcFile, "r")))
	{
		perror("error cf1 : cannot open src file");
		return 1;
	}
	if (!(f_dest = fopen(destFile, "w")))
	{
		perror("error cf2 : cannot open dest file");
		perror(destFile);		
		return 1;
 	}
 	
	int c;
	while ( (c = getc(f_src)) != EOF)
		if (putc(c, f_dest) == EOF)
		{
			perror("error cf3 : fail at copying");
			return 3;
		}

	if (fclose(f_src))
	{
		perror("error cf2 : cannot close src file");
		return 2;
	}
	if (fclose(f_dest))
	{
		perror("error cf2 : cannot close dest file");
		return 2;
	}
	
	struct stat fileStats;
	if (stat(srcFile, &fileStats))
	{
		perror("error cf4 : cannot read stats of src file");
		return 4;
	}
	chmod(destFile, fileStats.st_mode);
	return 0;
}




