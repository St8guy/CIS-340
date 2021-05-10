#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char*argv[])
{
	char mkdir[100];
	mkdir = argv[1];
	_mkdr(mkdir);
}

static void _mkdir(const char *dir) {
        char tmp[256];
        char p* = NULL;
        size_t len;

        snprintf(tmp, sizeof(tmp),"%s",dir); //if temp is '/' that would signify a new directory
        len = strlen(tmp);
        if(tmp[len - 1] == '/')
                tmp[len - 1] = 0;
        for(p = tmp + 1; *p; p++)
                if(*p == '/') {
                        *p = 0;
                        mkdir(tmp, S_IRWXU); //call the normal mkdir recursively
                        *p = '/';
                }
        mkdir(tmp, S_IRWXU);
}
share
