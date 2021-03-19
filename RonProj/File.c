#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "File.h"
#include "Compiler.h"

#define AS_STR ".as"
#define OB_STR ".ob"
#define ENT_STR ".ent"
#define EXT_STR ".ext"
#pragma warning(disable: 4996)

static FILE* inputFile = NULL;
static FILE* outputFile = NULL;

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
	int extraLength = maxLength + LINE_EXTRA_LENGTH;
	if (!str || !inputFile)
		return 0;
		
	if (!fgets(str, extraLength, inputFile))
	{
		return END;
	}

	if (strlen(str) > maxLength)
	{
		while (str && strlen(str) > maxLength && strlen(str) < extraLength - 1)
			fgets(str, extraLength, inputFile);

		return LINE_TOO_LONG;
	}

	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0'; /* remove \n in end of line */
	return OK;

}
int rewindInputFile()
{
	if (!inputFile)
		return -1;
	
	return fseek(inputFile, 0, 0);/*return 0*/ 
}


int createFileNames(char* filename, char** as, char** ob, char** ent, char** ext)
{
	*as = NULL;
	*ob = NULL;
	*ent = NULL;
	*ext = NULL;
	do
	{
		int origLength = strlen(filename);

		*as = (char*)malloc(origLength + strlen(AS_STR) + 1);
		if (!(*as))
			break;

		*ob = (char*)malloc(origLength + strlen(OB_STR) + 1);
		if (!(*ob))
			break;

		*ent = (char*)malloc(origLength + strlen(ENT_STR) + 1);
		if (!(*ent))
			break;

		*ext = (char*)malloc(origLength + strlen(EXT_STR) + 1);
		if (!(*ext))
			break;

		strcpy(*as, filename);
		strcat(*as, AS_STR);

		strcpy(*ob, filename);
		strcat(*ob, OB_STR);

		strcpy(*ent, filename);
		strcat(*ent, ENT_STR);

		strcpy(*ext, filename);
		strcat(*ext, EXT_STR);

		return 1;

	} while (0);

	/*error occured*/
	free(as);
	free(ob);
	free(ent);
	free(ext);

	return 0;
}

int openOutputFile(char* path)
{
	outputFile = fopen(path, "w");
	if (!outputFile)
		return 0;
	return 1;
}

void closeOutputFile()
{
	if (outputFile)
		fclose(outputFile);

	outputFile = NULL;
}

int writeOutput(char* str)
{
	return fputs(str, outputFile);

}
