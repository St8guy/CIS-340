#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST
void show_info(struct utmp *);
void showtime(long);
int main()
{
	struct utmp	utbuf;
	int		utmpfd;
	if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
	{
		perror(UTMP_FILE);
		exit(1);
	}
	while(read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
	{
		show_info(&utbuf);
	}
	close(utmpfd);
	return 0;
}
void show_info(struct utmp *utbufp)
{
	if ( utbufp -> ut_type != USER_PROCESS)
	{
		return;
	}
	printf("ut_name: %-8.8s ", utbufp -> ut_name);
	printf("ut_line: %-8.8s ", utbufp -> ut_line); //added a couple extra structs to output
	printf("ut_type: %d ", utbufp -> ut_type);
	showtime(utbufp ->ut_time);
#ifdef	SHOWHOST
	printf("ut_host: (%s)", utbufp -> ut_host);
#endif
	printf("\n"); 
}
void showtime(long timeval)
{
	char *cp;
	cp = ctime(&timeval);
	printf("ut_time: %12.12s ", cp+4);
}

