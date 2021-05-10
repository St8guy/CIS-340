#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main()
{
	struct dirent *dir;
	DIR *direc = opendir(".");

	if(direc == NULL)
	{
		printf("The current directory cannot be opened, please check your permissions");
		exit(1);
	}
	while ((dir = readdir(direc)) != NULL)
	{
		printf("%s\n", dir->d_name);
	}
	closedir(direc);
}
