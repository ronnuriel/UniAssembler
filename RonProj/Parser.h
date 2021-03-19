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

typedef enum
{
	PE_NO_ERROR = 0,
	PE_INVALID_SYM_NAME = 1,
	PE_INVALID_OPERATOR = 1 << 1,
	PE_INVALID_SOURCE_OPERAND = 1 << 2,
	PE_INVALID_TARGET_OPERAND = 1 << 2,
	PE_TOO_MANY_OPERANDS = 1 << 3,
	PE_COMMA_AT_START_OR_END = 1 << 4,
	PE_PARAM_NOT_A_NUM = 1 << 5,
	PE_PARAM_NOT_A_VALID_SYM = 1 << 6,
	PE_PARAM_NOT_A_STR = 1 << 7,
	PE_INVALID_INSTRUCTION = 1 << 8,
	PE_MISSING_PARAMS = 1 << 9
} ParserError;
typedef struct
{
	unsigned int error;
	int labelFlag;
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
	int labelFlag;
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