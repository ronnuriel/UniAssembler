#include "HelperFunc.h"
#include "Operator.h"
#include "Register.h"
#include "Symbol.h"

#include <string.h>
#define MAX_SYMBOL_LEN 31

int isValidSymbolName(char* symbol) {
	if (!symbol) 
		return 0;

	/* check length */
	if (strlen(symbol) > MAX_SYMBOL_LEN || strlen(symbol) == 0)
		return 0;

	/* check first letter */
	if (!isLetter(symbol[0]))
		return 0;

	/* check rest of letters */
	int i;
	for (i = 1; i < strlen(symbol); ++i)
	{
		if (!isAlphaNumeric(symbol[i]))
			return 0;
	}

	/* check if matches operator */
	if (operatorStringToEnum(symbol) != OPERATOR_INVALID)
		return 0;

	/* check if matches operator */
	if (isValidRegisterName(symbol))
		return 0;

	/* everything okay */
	return 1;
}


