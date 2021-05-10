#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char*argv[1])
{
	int num = 0;
	char word[100];
	char string[50];

	FILE *in_file = fopen(argv[1], "r"); //creates a pointer from the commandline of the file
	printf("Please enter the string you want to search for:\n");
	scanf("%s", word);

	if(in_file == NULL) //in the case of an error using the program
	{
		printf("There was an issue opening the requested file.\n");
		printf("Please try again with another filename.\n");
		exit(1);
	}
	printf("Here are all the words that begin with the given string.\n");

	while(!feof(in_file)) //will read through the file string by string
	{
		fscanf(in_file, "%s", string); //moves the current word to string
		if(strstr(string,word)) //compares string with word
		{
			printf("%s\n", string); //prints the string
		}
	}
}
