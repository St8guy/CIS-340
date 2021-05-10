#include      <stdio.h>
#include      <unistd.h>
#include      <stdlib.h>
#include	  <sys/wait.h>
#define DELAY 2

int main() 
{
	int n;
	printf("Please enter the number of child processes:\n");
	scanf("%d",&n);
	void child_code(), parent_code();
	int pid;
	for(int i=0;i<n;i++)
	{
		pid=fork();

		if(pid<0)
		{
			printf("\n Error ");
			exit(1);
		}
		else if(pid==0)
		{
			child_code(DELAY);
		}
		else
		{
			parent_code(pid);
		}
	}
}

void child_code(int delay) 
{
	printf("The child %d will sleep for %d seconds\n", getpid(), delay);
	sleep(delay);
	printf("Child done sleeping.\n");
	printf("\n");
	exit(17);
}

void parent_code(int childpid) 
{
	int wait_rv;
	wait_rv = wait(NULL);
	printf("Finished waiting for %d. Wait returned: %d\n", childpid, wait_rv);
}
