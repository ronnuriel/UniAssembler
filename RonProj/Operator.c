#include  <stdlib.h>
#include "Operator.h"
#include "Symbol.h"
#include "HelperFunc.h"

#include <string.h>

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
	{0, 0, operatorsNames[MOV], IMMEDIATE | DIRECT | REGISTER_DIRECT,     DIRECT | REGISTER_DIRECT},
	{1, 0, operatorsNames[CMP], IMMEDIATE | DIRECT | REGISTER_DIRECT,     IMMEDIATE | DIRECT | REGISTER_DIRECT},
	{2, 10, operatorsNames[ADD], IMMEDIATE | DIRECT | REGISTER_DIRECT,    DIRECT | REGISTER_DIRECT},
	{2, 11, operatorsNames[SUB], IMMEDIATE | DIRECT | REGISTER_DIRECT,    DIRECT | REGISTER_DIRECT},
	{4, 0, operatorsNames[LEA], DIRECT,                                   DIRECT | REGISTER_DIRECT},
	{5, 10, operatorsNames[CLR], NONE,                                    DIRECT | REGISTER_DIRECT},
	{5, 11, operatorsNames[NOT], NONE,                                    DIRECT | REGISTER_DIRECT},
	{5, 12, operatorsNames[INC], NONE,                                    DIRECT | REGISTER_DIRECT},
	{5, 13, operatorsNames[DEC], NONE,                                    DIRECT | REGISTER_DIRECT},
	{9, 10, operatorsNames[JMP], NONE,                                    DIRECT | RELATIVE},
	{9, 11, operatorsNames[BNE], NONE,                                    DIRECT | RELATIVE},
	{9, 12, operatorsNames[JSR], NONE,                                    DIRECT | RELATIVE},
	{12, 0, operatorsNames[RED], NONE,                                    DIRECT | REGISTER_DIRECT},
	{13, 0, operatorsNames[PRN], NONE,                                    IMMEDIATE | DIRECT | REGISTER_DIRECT},
	{14, 0, operatorsNames[RTS], NONE,                                    NONE},
	{15, 0,operatorsNames[STOP], NONE,                                    NONE}
};


OperatorsEnum operatorStringToEnum(char* str)
{
	if (!str)
	{
		return OPERATOR_INVALID;
	}
	else
	{
		int i;
		for (i = 0; i < NUM_OF_OPERATORS; i++)
		{
			if (!strcmp(str, operatorsNames[i])) /* found a match */
				return (OperatorsEnum)i;
		}

		/* didn't find a match */
		return OPERATOR_INVALID;
	}
}



int getOperatorOpcode(OperatorsEnum op)
{
	return getOperatorDataByEnum(op)->opcode;
}
int getOperatorFunct(OperatorsEnum op)
{
	return getOperatorDataByEnum(op)->funct;
}

int getAddrMethodBin(AddrMethodEnum method)
{
	switch (method)
	{
	case NONE:
	case IMMEDIATE:
		return 0;
	case DIRECT:
		return 1;
	case RELATIVE:
		return 2;
	case REGISTER_DIRECT:
		return 3;
	default:
		return -1;
	}

	return -1;
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
		if (isValidNum(str + 1))
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
	default:
		return;
	}
		
}

unsigned int generateBinaryWord(OperatorsEnum op, AddrMethodEnum source, AddrMethodEnum target)
{
	unsigned int opcodeBin = getOperatorOpcode(op) << 8;
	unsigned int functBin = getOperatorFunct(op) << 4;
	unsigned int sourceBin = getAddrMethodBin(source) << 2;
	unsigned int targetBin = getAddrMethodBin(target);

	return opcodeBin | functBin | sourceBin | targetBin;
}