#include <stdio.h>
#include <stdlib.h>

#include "File.h"

#pragma warning(disable: 4996)
static FILE* inputFile = NULL;

int openInputFile(char* path)
{

	inputFile = fopen(path, "r");
	if (!inputFile)
		return 0;
	return 1;
}
void closeInputFile()
{
	if (inputFile)
		fclose(inputFile);

	inputFile = NULL;
}
/* maxLength needs to be larger enough to have room for \n\0 */
int readNextLine(char* str, int maxLength)
{
	if (!str || !inputFile)
		return -1;

	if (fgets(str, maxLength, inputFile))
	{
		str[strlen(str)- 1] = '\0';
		return 1;
	}
		
	
	return 0;
}
int rewindInputFile()
{
	if (!inputFile)
		return -1;
	
	return fseek(inputFile, 0, 0);//return 0 
}