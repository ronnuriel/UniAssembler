#include "HelperFunc.h"
#include <stdlib.h>
#include <string.h>
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

int isWhiteSpace(char ch)
{
	return (ch == ' ' || ch == '\t');
}

/* checks if the number is vaild */
int isValidNumericSign(char ch)
{
	return(ch == '+' || ch == '-');
}

int isValidNum(char* str)
{
	if (!str || strlen(str) == 0)
	{
		return 0;
	}
	else if (isValidNumericSign(str[0]) && strlen(str) == 1)
	{
		return 0;
	}
	else if (!isValidNumericSign(str[0]) && !isNumeric(str[0])) /* first char not +,-, or digit*/
	{
		return 0;
	}
	else
	{
		unsigned int i;
		for (i = 1; i < strlen(str); ++i)
		{
			if (!isNumeric(str[i])) {
				return 0;
			}
		}

		return 1;
	}
}

int charDigitToInt(char digit)
{
	return digit - '0';
}

int isWhiteSpacesLine(char* str)
{
	while (*str != '\0')
	{
		if (!isWhiteSpace(*str))
			return 0;
		str++;
	}
	return 1;
}
char* removeLeadingSpaces(char* str)
{
	if (!str)
		return NULL;
	while (*str != '\0' && isWhiteSpace(*str))
		str++;

	return str;
}


void removeTrailingSpaces(char* str)
{
	if (!str)
		return;

	int i;

	for (i = strlen(str) - 1; i >= 0 && isWhiteSpace(str[i]); i--);
	str[i + 1] = '\0';
 
}

/* Function to remove all spaces from a given string */
void removeSpaces(char* str)
{
	/* To keep track of non-space character count */
	int count = 0;

	/* Traverse the given string. If current character */
	/* is not space, then place it at index 'count++' */
	int i;
	for(i=0; str[i]; i++)
		if (str[i] != ' ')
			str[count++] = str[i]; /* here count is */
								   /* incremented */
	str[count] = '\0';
}
int countOccurrencesInString(char c, char* str)
{
	
	int i, counter = 0;
	for (i = 0; i < strlen(str); i++)
		if (str[i] == c)
			counter++;
	return counter;

	
}