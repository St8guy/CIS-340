#include<stdio.h>
#include<string.h>

int flag=0;

int Hello()
{
if (flag==0 || flag==1)
  {
     printf("Hello\n");
     flag = 2;
     return 1;
  }
else
  return 0;
}

int World()
{
if (flag==0 || flag==2)
  {
    printf("World\n");
    flag=1;
    return 1;
  }
else
  return 0;
}


