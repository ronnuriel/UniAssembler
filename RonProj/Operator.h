#ifndef OPERATOR_H
#define OPERATOR_H
#include "Register.h"
/* check */
typedef enum
{
	OPERATOR_INVALID = -1,
	MOV = 0,
	CMP,
	ADD,
	SUB,
	LEA,
	CLR,
	NOT,
	INC,
	DEC,
	JMP,
	BNE,
	JSR,
	RED,
	PRN,
	RTS,
	STOP,
	NUM_OF_OPERATORS
} OperatorsEnum;

//extern char operatorsNames[][NUM_OF_OPERATORS];

OperatorsEnum operatorStringToEnum(char* str);



typedef enum 
{
	NONE = 0,
	IMMEDIATE = 0x1,
	DIRECT = (0x1<<1),
	RELATIVE = (0x1 << 2),
	REGISTER_DIRECT = (0x1 << 3),
	INVALID_ADDR_METHOD = (0x1 << 7)
} AddrMethodEnum;


typedef struct 
{
	int opcode;
	int funct;
	char* name;
	unsigned char sourceOperandAddrMethods; /* bitwise storage */
	unsigned char destOperandAddrMethods; /* bitwise storage */
} OperatorData;

//extern OperatorData operators[];

int getOperatorOpcode(OperatorsEnum op);
int getOperatorFunct(OperatorsEnum op);
int getAddrMethodBin(AddrMethodEnum method);
char* getOperatorName(OperatorsEnum op);
int isSourceAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);
int isDestAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);

OperatorData* getOperatorDataByEnum(OperatorsEnum op);

AddrMethodEnum detectOperandType(char* str);
void stripOperandData(char* dest, char* source, AddrMethodEnum);
unsigned int generateBinaryWord(OperatorsEnum op, AddrMethodEnum source, AddrMethodEnum target);
#endif