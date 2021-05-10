#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
void binary(long int n);

int main(int argc, char *argv[])
{
	struct stat	fileinfo;
	int		i;
	if(argc != 2)
	{
		printf("Usage: statfile filename\n");
		exit(0);
	}
	i=stat(argv[1], &fileinfo);
	if(i == -1)
	{
		printf("Unable to stat %s\n", argv[1]);
		exit(0);
	}
	printf("size: %ld\n", fileinfo.st_size);
	printf("permissions: %d\n", fileinfo.st_mode);
	printf("last modified: %ld\n", fileinfo.st_mtime);
	binary(fileinfo.st_mode);
}
void binary(long int n)
{
	int i;
	printf("Permissions in Binary:");
	for(i = 1 << 9; i > 0; i = i/2)
	{
		(n & i)? printf("1"): printf("0");
	}
	printf("\n");
}
