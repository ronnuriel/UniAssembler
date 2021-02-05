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