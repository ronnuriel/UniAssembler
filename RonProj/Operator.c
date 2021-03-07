#include  <stdlib.h>
#include "Operator.h"
#include <string.h>
#pragma warning(disable: 4996)
static char operatorsNames[][NUM_OF_OPERATORS] = {
	"mov",
	"cmp",
	"add",
	"sub",
	"lea",
	"clr",
	"not",
	"inc",
	"dec",
	"jmp",
	"bne",
	"jsr",
	"red",
	"prn",
	"rts",
	"stop"
};

static OperatorData operators[NUM_OF_OPERATORS] = {
	{0, -1, operatorsNames[MOV], IMMEDIATE | DIRECT | REGISTER_DIRECT, DIRECT | REGISTER_DIRECT},
	{1, -1, operatorsNames[CMP], IMMEDIATE | DIRECT | REGISTER_DIRECT, IMMEDIATE | DIRECT | REGISTER_DIRECT},
	{2, 10, operatorsNames[ADD], IMMEDIATE | DIRECT | REGISTER_DIRECT, DIRECT | REGISTER_DIRECT},
	{2, 11, operatorsNames[SUB], IMMEDIATE | DIRECT | REGISTER_DIRECT, DIRECT | REGISTER_DIRECT},
	{4, -1, operatorsNames[LEA], DIRECT,                               DIRECT | REGISTER_DIRECT},
	{5, 10, operatorsNames[CLR], 0,                                    DIRECT | REGISTER_DIRECT},
	{5, 11, operatorsNames[NOT], 0,                                    DIRECT | REGISTER_DIRECT},
	{5, 12, operatorsNames[INC], 0,                                    DIRECT | REGISTER_DIRECT},
	{5, 13, operatorsNames[DEC], 0,                                    DIRECT | REGISTER_DIRECT},
	{9, 10, operatorsNames[JMP], 0,                                    DIRECT | RELATIVE},
	{9, 11, operatorsNames[BNE], 0,                                    DIRECT | RELATIVE},
	{9, 12, operatorsNames[JSR], 0,                                    DIRECT | RELATIVE},
	{12, -1, operatorsNames[RED],0,                                    DIRECT | REGISTER_DIRECT},
	{13, -1, operatorsNames[PRN], 0,                                   IMMEDIATE | DIRECT | REGISTER_DIRECT},
	{14, -1, operatorsNames[RTS], 0,                                   0},
	{15, -1,operatorsNames[STOP], 0,                                   0}
};


OperatorsEnum operatorStringToEnum(char* str)
{
	if (!str)
		return OPERATOR_INVALID;
	int i;
	for (i = 0; i < NUM_OF_OPERATORS; i++)
	{
		if (!strcmp(str, operatorsNames[i])) /* found a match */
			return (OperatorsEnum)i;
	}

	/* didn't find a match */
	return OPERATOR_INVALID;
}



int getOperatorOpcode(OperatorsEnum op)
{
	return getOperatorDataByEnum(op)->opcode;
}
int getOperatorFunct(OperatorsEnum op)
{
	return getOperatorDataByEnum(op)->funct;
}

char* getOperatorName(OperatorsEnum op)
{
	return getOperatorDataByEnum(op)->name;
}

int isSourceAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method)
{
	return (getOperatorDataByEnum(op)->sourceOperandAddrMethods & method) ? 1 : 0;
}

int isDestAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method)
{
	return (getOperatorDataByEnum(op)->destOperandAddrMethods & method) ? 1 : 0;
}


OperatorData* getOperatorDataByEnum(OperatorsEnum op)
{
	return &operators[op];
}

AddrMethodEnum detectOperandType(char* str)
{
	if (isValidRegisterName(str))
		return REGISTER_DIRECT;

	if (str[0] == '#')
	{
		if (isVaildNum(str + 1))
			return IMMEDIATE;
		else
			return INVALID_ADDR_METHOD;
	}
	if (str[0] == '%')
	{
		if (isValidSymbolName(str + 1))
			return RELATIVE;
		else
			return INVALID_ADDR_METHOD;
	}
	/* suspect direct*/
	if (isValidSymbolName(str))
		return DIRECT;
	else
		return INVALID_ADDR_METHOD;

		 
}
void stripOperandData(char* dest, char* source, AddrMethodEnum addrMethod)
{
	switch (addrMethod)
	{
	case DIRECT:
	case REGISTER_DIRECT:
		strcpy(dest, source);
		return;

	case IMMEDIATE:
	case RELATIVE:
		strcpy(dest, source+1);
		return;
	}
}