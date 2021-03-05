#include "HelperFunc.h"
#include <stdlib.h>
#include <string.h>
#define AS_STR ".as"
#define OB_STR ".ob"
#define ENT_STR ".ent"
#define EXT_STR ".ext"
#pragma warning(disable: 4996)
int isLetter(char ch)
{
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}
int isNumeric(char ch) {
	return (ch >= '0' && ch <= '9');
}

int isAlphaNumeric(char ch) 
{
	return (isLetter(ch) || isNumeric(ch));
}

/* checks if the number is vaild */
int isValidNumericSign(char ch)
{
	return(ch == '+' || ch == '-');
}

int isVaildNum(char* str)
{
	if (!str || strlen(str) == 0) 
		return 0;
	if (isValidNumericSign(str[0]) && strlen(str) == 1)
		return 0;

	if (!isValidNumericSign(str[0]) && !isNumeric(str[0])) // first char not +,-, or digit
		return 0;

	int i;
	for (i = 1; i < strlen(str); ++i)
	{
		if (!isNumeric(str[i])) {
			return 0;
		}
	}

	return 1; 
}

int charDigitToInt(char digit)
{
	return digit - '0';
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

	// error occured
	free(as);
	free(ob);
	free(ent);
	free(ext);

	return 0;
}