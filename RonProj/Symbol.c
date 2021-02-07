
#include "HelperFunc.h"
#include "Operator.h"
#define MAX_SYMBOL_LEN 31

int isValid(char* symbol) {
	if (!symbol) 
		return 0;

	/* check length */
	if (sizeof(symbol) > MAX_SYMBOL_LEN || sizeof(symbol) == 0)
		return 0;

	/* check first letter */
	if (!isLetter(symbol[0]))
		return 0;

	/* check rest of letters */
	int i;
	for (i = 1; i < sizeof(symbol); ++i)
	{
		if (!isAlphaNumeric(symbol[i]))
			return 0;
	}

	/* check if matches operator */
	if (operatorStringToEnum(symbol) != OPERATOR_INVALID)
		return 0;

	/* check if matches operator */
	if (!isValidRegisterName(symbol))
		return 0;

	/* everything okay */
	return 1;
}


