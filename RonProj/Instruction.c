#include "Instruction.h"
InstructionTypeEnum InstructionTypeStringToEnum(char* str)
{
	if (strcmp(str, INSTRUCTION_TYPE_DATA_STR))
		return INST_TYPE_DATA;
	if (strcmp(str, INSTRUCTION_TYPE_ENTRY_STR))
		return INST_TYPE_ENTRY;
	if (strcmp(str, INSTRUCTION_TYPE_STRING_STR))
		return INST_TYPE_STRING;
	if (strcmp(str, INSTRUCTION_TYPE_EXTERN_STR))
		return INST_TYPE_EXTERN;

	return INST_TYPE_INVALID;
}