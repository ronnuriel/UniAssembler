#include <string.h>
#include "Parser.h"
#include <string.h>
#include "HelperFunc.h"
#include "Symbol.h"
#include "Operator.h"
#include "Instruction.h"
#include <stdio.h>
#include <malloc.h>




LineTypeEnum detectLineType(char* line)
{
	if (!line)
	{
		return INVALID_LINE;
	}
	if (line[0] == '\0')/*EMPTY_LINE - strlen = 0*/
	{
		return EMPTY_LINE;
	}
	if (line[0] == ';')/*COMMENT_LINE*/
	{
		return COMMENT_LINE;
	}
	if (isWhiteSpacesLine(line)) /*EMPTY_LINE - filled with whitespaces*/
	{
		return EMPTY_LINE;
	}
	if (strchr(line,'.'))/*INSTRUCTION_LINE*/
	{
		return INSTRUCTION_LINE;
	}
	
	return OPERATION_LINE;
}


Instruction* parseIntruction(char* line)
{

	Instruction* ret = (Instruction*)malloc(sizeof(Instruction));
	char* label = NULL;
	char* instructionTypeStr = NULL;
	if (!ret)
		return NULL;
	
	ret->error = 0;
	ret->labelFlag = 0;
	ret->label[0] = '\0'; /* empty label*/
	
	ret->type = INST_TYPE_INVALID;
	ret->params = NULL;
	ret->numParams = 0;

	
	line = removeLeadingSpaces(line);

	/* search for LABEL*/
	if (strchr(line, ':'))
	{
		label = strtok(line, ":");
		if (!isValidSymbolName(label))
		{
			/* Error: invalid label */
			ret->error |= PE_INVALID_SYM_NAME;
		}
		else
		{
			strcpy(ret->label, label);
			ret->labelFlag = 1;
			/* prepeare line for next parsing */
		}
		line = strtok(NULL, "");
	}



	instructionTypeStr = strtok(line, " ");
	instructionTypeStr = removeLeadingSpaces(instructionTypeStr);
	removeTrailingSpaces(instructionTypeStr);
	ret->type = InstructionTypeStringToEnum(instructionTypeStr);
	
	line = strtok(NULL, "");
	line = removeLeadingSpaces(line);
	removeTrailingSpaces(line);
	
	switch (ret->type)
	{
	case INST_TYPE_DATA:
	{
		int i;
		char* param = NULL;
		if (line == NULL)
		{
			ret->error |= PE_MISSING_PARAMS;
			return ret;
		}
		if (line[0] == ',' || line[strlen(line) - 1] == ',')
		{

			/* comma at start or end. error*/
			ret->error |= PE_COMMA_AT_START_OR_END;
			return ret;
		}

		ret->numParams = countOccurrencesInString(',', line) + 1;
		ret->params = (char**)malloc(sizeof(char*) * ret->numParams);
		if (!ret->params)
		{
			/* allocation failed*/
			free(ret);
			return NULL;
		}

		
		param = strtok(line, ",");
		for (i = 0; i < ret->numParams; i++)
		{
			param = removeLeadingSpaces(param);
			removeTrailingSpaces(param);
			if (!isValidNum(param))
			{
				int j;
				/* not a number. deallocate.*/
								
				for (j = 0; j < i; j++)
					free(ret->params[j]);
				free(ret->params);
				ret->params = NULL;
				ret->numParams = 0;

				ret->error |= PE_PARAM_NOT_A_NUM;
				return ret;
			}

			ret->params[i] = (char*)malloc(strlen(param) + 1);
			if (!ret->params[i])
			{
				int j;
				
				/*malloc failed. deallocate.*/
				
				for (j = 0; j < i; j++)
					free(ret->params[j]);

				free(ret->params);
				free(ret);
				
				return NULL;
			}

			strcpy(ret->params[i], param);

			param = strtok(NULL, ",");
		}
		return ret;
		
	}
	case INST_TYPE_ENTRY:
	case INST_TYPE_EXTERN:
	{
		if (line == NULL)
		{
			ret->error |= PE_MISSING_PARAMS;
			return ret;
		}
		if (!isValidSymbolName(line))
		{
			
			/* Error: param is not a valid symbol */
			ret->error |= PE_PARAM_NOT_A_VALID_SYM;
			
		}
		
		/* copy label*/
		ret->params = (char**)malloc(sizeof(char*) * 1);
		if (!ret->params)
		{
			/* malloc failed*/
			free(ret);
			return NULL;
		}


		ret->params[0] = malloc((sizeof(char) * strlen(line)) + 1);
		if (!ret->params[0])
		{
			
			/* malloc failed*/
			free(ret->params);
			free(ret);
			return NULL;
		}
		strcpy(ret->params[0], line);
		ret->numParams = 1;

		return ret;
	}
	
	case INST_TYPE_STRING:
	{
		if (line == NULL)
		{
			ret->error |= PE_MISSING_PARAMS;
			return ret;
		}
		if (line[0] != '\"' || line[strlen(line) - 1] != '\"')
		{
			/* Error: invalid data string */
			ret->error |= PE_PARAM_NOT_A_STR;
			return ret;
		}
		/* copy string from pos 1 to strlen-2*/
		
		/* remove " */
		line++;
		line[strlen(line) - 1] = '\0';

		ret->params = (char**)malloc(sizeof(char*) * 1);
		if (!ret->params)
		{
			/* malloc failed*/
			free(ret);
			return NULL;
		}

		
		ret->params[0] = malloc((sizeof(char) * strlen(line))+ 1);
		if (!ret->params[0])
		{
			/* malloc failed*/
			free(ret->params);
			free(ret);
			
			return NULL;
		}
		strcpy(ret->params[0], line);
		ret->numParams = 1;

		return ret;
	}

	case INST_TYPE_INVALID:
	{
		ret->error |= PE_INVALID_INSTRUCTION;
		return ret;
	}
	}



	return ret;
}

Operation* parseOperation(char* line)
{
	Operation* ret = (Operation*)malloc(sizeof(Operation));
	char* label = NULL,
		* opcodeStr = NULL,
		* operand1 = NULL,
		* operand2 = NULL;

	if (!ret)
		return NULL;

	ret->error = 0;
	ret->label[0] = '\0'; /* empty label*/
	ret->labelFlag = 0;
	ret->sourceType = NONE;
	ret->targetType = NONE;
	ret->source[0] = '\0';
	ret->target[0] = '\0';


	line = removeLeadingSpaces(line);

	/* search for LABEL*/
	if (strchr(line, ':'))
	{
		label = strtok(line, ":");
		if (!isValidSymbolName(label))
		{
			/* Error: invalid label */
			ret->error |= PE_INVALID_SYM_NAME;
		}
		strcpy(ret->label, label);
		ret->labelFlag = 1;
		/* prepeare line for next parsing */
		line = strtok(NULL, "");

	}
	
	
	opcodeStr = strtok(line, " ");
	/* TODO: what if opcodeStr is NULL??*/
	opcodeStr = removeLeadingSpaces(opcodeStr);
	removeTrailingSpaces(opcodeStr);

	line = strtok(NULL, "");
	ret->opcode = operatorStringToEnum(opcodeStr);
	if (ret->opcode == OPERATOR_INVALID)
	{
		/* Error: invalid operand */
		ret->error |= PE_INVALID_OPERATOR;
	}

	

	operand1 = strtok(line, ",");
	if (operand1 && !isWhiteSpacesLine(operand1))
	{

		operand1 = removeLeadingSpaces(operand1);
		removeTrailingSpaces(operand1);
		

		operand2 = strtok(NULL, ",");
		if (operand2 && !isWhiteSpacesLine(operand2))
		{
			operand2 = removeLeadingSpaces(operand2);
			removeTrailingSpaces(operand2);
		
		}
		else
		{
			operand2 = NULL;
		}
	}
	else
	{
		operand1 = NULL;
	}
	/* now we know how many operands there is*/

	
	if (operand1 && operand2)
	{
		ret->targetType = detectOperandType(operand2);
		stripOperandData(ret->target, operand2, ret->targetType);
		ret->sourceType = detectOperandType(operand1);
		stripOperandData(ret->source, operand1, ret->sourceType);
	}
	else if (operand1)
	{
		ret->targetType = detectOperandType(operand1);
		stripOperandData(ret->target, operand1, ret->targetType);
	}

	if (ret->sourceType == INVALID_ADDR_METHOD)
	{
		ret->error |= PE_INVALID_SOURCE_OPERAND;
	}
	if (ret->targetType == INVALID_ADDR_METHOD)
	{
		ret->error |= PE_INVALID_TARGET_OPERAND;
	}
	
	if (strtok(NULL, ","))
	{
		/* more than 2 operands found */
		ret->error |= PE_TOO_MANY_OPERANDS;
	}
	
	return ret;
}



void freeInstruction(Instruction* instruction)
{
	if (instruction)
	{
		int i;
		for (i = 0; i < instruction->numParams; i++)
		{
			free(instruction->params[i]);
		}
		free(instruction->params);
		free(instruction);
	}
}

void freeOperation(Operation* operation)
{
	free(operation);
}