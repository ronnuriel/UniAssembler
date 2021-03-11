#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define INSTRUCTION_TYPE_DATA_STR ".data"
#define INSTRUCTION_TYPE_ENTRY_STR ".entry"
#define INSTRUCTION_TYPE_STRING_STR ".string"
#define INSTRUCTION_TYPE_EXTERN_STR ".extern"

typedef enum
{
	INST_TYPE_INVALID = -1,
	INST_TYPE_DATA,
	INST_TYPE_ENTRY,
	INST_TYPE_STRING,
	INST_TYPE_EXTERN
} InstructionTypeEnum;

InstructionTypeEnum InstructionTypeStringToEnum(char* str);

#endif