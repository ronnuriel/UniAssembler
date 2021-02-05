#include <stdio.h>
#include "Operator.h"
int main()
{
	char str[] = "bne";
	OperatorsEnum e = operatorStringToEnum(str);
	printf("%d", getOperatorFunct(e));
	return 1;
}