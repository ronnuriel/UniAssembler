#ifndef OPERATOR_H
#define OPERATOR_H

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

extern char operatorsNames[][NUM_OF_OPERATORS];

OperatorsEnum operatorStringToEnum(char* str);



typedef enum 
{
	IMMEDIATE = 0x1,
	DIRECT = (0x1<<1),
	RELATIVE = (0x1 << 2),
	REGISTER_DIRECT = (0x1 << 3)

} AddrMethodEnum;


typedef struct 
{
	int opcode;
	int funct;
	char* name;
	unsigned char sourceOperandAddrMethods; // bitwise storage
	unsigned char destOperandAddrMethods; // bitwise storage
} OperatorData;

extern OperatorData operators[];

int getOperatorOpcode(OperatorsEnum op);
int getOperatorFunct(OperatorsEnum op);
char* getOperatorName(OperatorsEnum op);
int isSourceAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);
int isDestAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);

OperatorData* getOperatorDataByEnum(OperatorsEnum op);
#endif