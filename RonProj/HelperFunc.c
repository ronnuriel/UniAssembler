#include "HelperFunc.h"

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