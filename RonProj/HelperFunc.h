#ifndef HELPER_FUNC_H
#define HELPER_FUNC_H

/*Checked*/

int isLetter(char ch);
int isNumeric(char ch);
int isAlphaNumeric(char ch);
int isWhiteSpace(char ch);
int isValidNumericSign(char ch);
int isVaildNum(char* str);



int charDigitToInt(char digit);

int isWhiteSpacesLine(char* str);
char* removeLeadingSpaces(char* str);
void removeTrailingSpaces(char* str); 
void removeSpaces(char* str);
int countOccurrencesInString(char c, char* str);
#endif