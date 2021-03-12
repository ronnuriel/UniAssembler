#ifndef PARSER_FUNC
#define PARSER_FUNC
#include "Symbol.h"
#include "Operator.h"
#include "Instruction.h"
typedef enum 
{
	INVALID_LINE = -1, 
	EMPTY_LINE,
	COMMENT_LINE,
	INSTRUCTION_LINE,
	OPERATION_LINE
} LineTypeEnum;

typedef struct
{
	int error;
	char label[MAX_SYMBOL_LEN];
	OperatorsEnum opcode;
	AddrMethodEnum sourceType;
	char source[MAX_SYMBOL_LEN];
	AddrMethodEnum targetType;
	char target[MAX_SYMBOL_LEN];
} Operation;

typedef struct
{
	int error;
	char label[MAX_SYMBOL_LEN];
	InstructionTypeEnum type;

	char** params;
	int numParams;
} Instruction;

void freeInstruction(Instruction* instruction);
void freeOperation(Operation* operation);


LineTypeEnum detectLineType(char* line);

Instruction* parseIntruction(char* line);
Operation *parseOperation(char* line);

AddrMethodEnum detectOperandType(char* str);
#endif