#include "Register.h"
#include "HelperFunc.h"
#define REGISTER_LEN 2
#define MAX_REGISTER_NUM 7



/* check if the Register is valid  */
int isValidRegisterName(char* str) 
{
	return (sizeof(str) == REGISTER_LEN &&
		    str[0] == 'r' && 
		    isNumeric(str[1]) && 
		    charDigitToInt(str[1]) <= MAX_REGISTER_NUM);
}
	

