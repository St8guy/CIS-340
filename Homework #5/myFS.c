#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void do_ls(char dirname[], int sortEntry);
void dostat(char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

int main(int ac,char *av[])
{
    /*
    Sort flags
    
    default = 0:
    modtime = 1: -t    
    size =    2: -s
    name =    3: -n
    */
    
    if(ac > 1)
    {
        if(strcmp(av[1], "ls") == 0)
        {
            int sortTag = 0;
            int i;
            char *path;
    
            if(ac > 1)
                for(i = 2; i < ac; i++)
                {
                    if(strcmp(av[i], "-l") == 0) {
                        sortTag = 1;
                    }
                    else if (strcmp(av[i], "-t") == 0) {
                        sortTag = 3;
                    }
                    else if (strcmp(av[i], "-s") == 0) {
                        sortTag = 2;
                    }
                    else if (strcmp(av[i], "-n") == 0) {
                        sortTag = 1;
                    }
                    else if (strcmp(av[i], "-l") != 0 && strcmp(av[i], "-t") != 0 && strcmp(av[i], "-s") != 0 && strcmp(av[i], "-n") != 0){
                        path = (char *) malloc (strlen(av[i]));
                        strcpy(path, av[i]);
                    }
                }
        
            if(path != NULL)
            {
                chdir(path);
                printf("Path: %s\n", path);
                do_ls(".", sortTag);
            }
            else
            {
                do_ls(".", sortTag);
            }
    
            free(path);
        }
        else if(strcmp(av[1], "cp") == 0)
        {
            int in_fd, out_fd, n_chars;
            char buf[BUFFERSIZE];
            
            if(ac != 4)
            {
                printf("Error. Not enough Arguments\n");
                exit(1);
            }
            
            if((in_fd = open(av[2], O_RDONLY)) == -1)
            {
                printf("Cannot open file\n");
                exit(1);
            }
            
            if((out_fd = creat(av[3], COPYMODE)) == -1)
            {
                printf("Cannot create output file\n");
                exit(1);
            }
            
            while ( (n_chars = read(in_fd, buf, BUFFERSIZE)) > 0 ) {
                if( write( out_fd, buf, n_chars ) != n_chars )
                {
                    printf("Write Error\n");
                }
            }
            
            if ( close(in_fd) == -1 || close(out_fd) == -1 )
            {
                printf("Failed to Close Files\n");
            }
        }
        else
        {
            printf("Invalid Command\n");
        }
    }
    else
    {
        printf("Invalid Command\n");
    }
}

void do_ls(char dirname[], int sortEntry)
{
    DIR *dir_ptr;
    struct dirent *direntp;
    if((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "cannot open %s\n", dirname);
    }
    else
    {            
        int count = 0;
        int i, j, k;
        int swapInt;
        char swapString[500];
        struct stat info;
        
        while((direntp = readdir(dir_ptr)) != NULL)
        {
            count++;
        }
        
        closedir(dir_ptr);
            
        char nameArray[count][500];
        int indexArray[count];
        long int sizeArray[count];
        int timeArray[count];
        
        dir_ptr = opendir(dirname);
        
        for (i = 0; i < count; i++)
        {
            direntp = readdir(dir_ptr);
            stat(direntp->d_name, &info);
            sizeArray[i] = info.st_size;
            timeArray[i] = info.st_mtime;
            strcpy(nameArray[i], direntp->d_name);
            indexArray[i] = i;
        }
        
        closedir(dir_ptr);

        //Sort by name
        if (sortEntry == 1)
        {
            for (i = 0; i < count - 1; i++)
            {
                for (j = 0; j < (count - 1 - i); j++)
                {
                    if (strcmp(nameArray[j], nameArray[j + 1]) > 0)
                    {                                    
                        strcpy(swapString, nameArray[j]);
                        strcpy(nameArray[j], nameArray[j + 1]);
                        strcpy(nameArray[j + 1], swapString);
                    }
                }
            }
        }    
        //Sort by filesize
        else if (sortEntry == 2)
        {
            for (i = 0; i < count - 1; i++)
            {
                for (j = 0; j < (count - 1 - i); j++)
                {
                    if (sizeArray[j] > sizeArray[j + 1])
                    {                        
                        swapInt = sizeArray[j];
                        sizeArray[j] = sizeArray[j + 1];
                        sizeArray[j + 1] = swapInt;
                    
                        swapInt = indexArray[j];
                        indexArray[j] = indexArray[j + 1];
                        indexArray[j + 1] = swapInt;
                    }
                }
            }
        }
        //sort by time
        else if (sortEntry == 3)
        {
            for (i = 0; i < count - 1; i++)
            {
                for (j = 0; j < (count - 1 - i); j++)
                {
                    if (timeArray[j] > timeArray[j + 1])
                    {
                        swapInt = timeArray[j];
                        timeArray[j] = timeArray[j + 1];
                        timeArray[j + 1] = swapInt;
                        
                        swapInt = indexArray[j];
                        indexArray[j] = indexArray[j + 1];
                        indexArray[j + 1] = swapInt;
                    }
                }
            }
        }
            
        //Print it all
        int sortedIndex;
        for(i = 0; i < count; i++)
        {
            sortedIndex = indexArray[i];
        
            if (sortEntry != 0)
            {
                dostat(nameArray[sortedIndex]);
            }
            else
            {
                dostat(nameArray[i]);
            }
        }
    }
}

void dostat(char *filename)
{
    struct stat info;
    if(stat(filename, &info) == -1)
    {
        perror(filename);
    }
    else
    {
        show_file_info(filename, &info);
    }
}

void show_file_info(char *filename, struct stat * info_p)
{
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11];
    mode_to_letters(info_p->st_mode, modestr);
    printf("%s", modestr);
    printf("%4d ", (int)info_p->st_nlink);
    printf("%-8s ", uid_to_name(info_p->st_uid));
    printf("%-8s ", gid_to_name(info_p->st_gid));
    printf("%8ld ", (long)info_p->st_size);
    printf("%.12s ", 4 + ctime(&info_p->st_mtime));
    printf("%s\n", filename);
}

void mode_to_letters (int mode, char str[])
{
    strcpy( str, "----------" );
    
    if ( S_ISDIR(mode) ) str[0] = 'd';
    if ( S_ISCHR(mode) ) str[0] = 'c';
    if ( S_ISBLK(mode) ) str[0] = 'b';
    
    if ( mode & S_IRUSR ) str[1] = 'r';
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( mode & S_IXUSR ) str[3] = 'x';
    
    if ( mode & S_IRGRP ) str[4] = 'r';
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( mode & S_IXGRP ) str[6] = 'x';
    
    if ( mode & S_IROTH ) str[7] = 'r';
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( mode & S_IXOTH ) str[9] = 'x';
}

char *uid_to_name( uid_t uid )
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];
    
    if( ( pw_ptr = getpwuid( uid ) ) == NULL )
    {
        sprintf(numstr, "%d", uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}

char *gid_to_name (gid_t gid)
{
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];
    
    if ( ( grp_ptr = getgrgid(gid) ) == NULL )
    {
        sprintf(numstr, "d", gid);
        return numstr;
    }
        return grp_ptr->gr_name;
}    


