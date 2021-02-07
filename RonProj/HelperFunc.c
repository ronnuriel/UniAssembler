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
int isValidSignum(char ch)
{
	return(ch == '+' || ch == "-");
}

int isVaildNum(char* str)
{
	int i;
	for (i = 0; i < sizeof(str); ++i)
	{
		if (!isValidSignum(str[i]) && !isNumeric(str[i])) {
			return 0;
		}
	}
	return 1; 
}
