#include "Register.h"
#include "HelperFunc.h"
#define REGISTER_LEN 2
#define MAX_REGISTER_NUM 7



/* check if the Register is valid  */
int isValidRegister(char* str) 
{
	if (sizeof(str) != REGISTER_LEN) {
		return 0;
	}
	if (isNumeric(str[1]))
	{
		return(str[0] == 'r'&& str[1]<=MAX_REGISTER_NUM);
	}
	return 0;


	
	
}