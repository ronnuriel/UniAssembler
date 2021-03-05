#ifndef HELPER_FUNC_H
#define HELPER_FUNC_H

/*Checked*/

int isLetter(char ch);
int isNumeric(char ch);
int isAlphaNumeric(char ch);


int isValidNumericSign(char ch);
int isVaildNum(char* str);

int charDigitToInt(char digit);

int createFileNames(char* filename, char** as, char** ob, char** ent, char** ext);
#endif