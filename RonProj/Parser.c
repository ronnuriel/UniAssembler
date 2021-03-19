#include <string.h>
#include "Parser.h"
#include <string.h>
#include "HelperFunc.h"
#include "Symbol.h"
#include "Operator.h"
#include "Instruction.h"
#include <stdio.h>

#pragma warning(disable: 4996)



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
	if (!ret)
		return NULL;
	printf("line 1: %s\n", line);
	ret->error = 0;
	ret->labelFlag = 0;
	ret->label[0] = '\0'; /* empty label*/
	
	ret->type = INST_TYPE_INVALID;
	ret->params = NULL;
	ret->numParams = 0;

	char* label = NULL;

	line = removeLeadingSpaces(line);

	/* search for LABEL*/
	if (strchr(line, ':'))
	{
		label = strtok(line, ":");
		if (!isValidSymbolName(label))
		{
			/* Error: invalid label */
			ret->error = 2;
			return ret;
		}
		strcpy(ret->label, label);
		ret->labelFlag = 1;
		/* prepeare line for next parsing */
		line = strtok(NULL, "");

		printf("line 2: %s\n", line);

	}



	char *instructionTypeStr = strtok(line, " ");
	instructionTypeStr = removeLeadingSpaces(instructionTypeStr);
	removeTrailingSpaces(instructionTypeStr);
	ret->type = InstructionTypeStringToEnum(instructionTypeStr);
	
	line = strtok(NULL, "");
	line = removeLeadingSpaces(line);
	removeTrailingSpaces(line);
	printf("line 3: %s\n", line);
	switch (ret->type)
	{
	case INST_TYPE_DATA:
	{
		if (line[0] == ',' || line[strlen(line) - 1] == ',')
		{
			/* comma at start or end. error*/
		}
		ret->numParams = countOccurrencesInString(',', line) + 1;
		ret->params = (char**)malloc(sizeof(char*) * ret->numParams);
		if (!ret->params)
		{
			printf("malloc failed - params\n");
			ret->error = 10;
			return ret;
		}

		int i;
		char* param = strtok(line, ",");
		for (i = 0; i < ret->numParams; i++)
		{
			param = removeLeadingSpaces(param);
			removeTrailingSpaces(param);
			if (!isValidNum(param))
			{
				/* not a number. deallocate.*/
				printf("not a valid num\n");
				int j;
				for (j = 0; j < i; j++)
					free(ret->params[j]);
				free(ret->params);

				ret->error = 6;
				return ret;
			}

			ret->params[i] = (char*)malloc(strlen(param) + 1);
			if (!ret->params[i])
			{
				printf("malloc failed - params[%d]\n", i);
				/*malloc failed. deallocate.*/
				int j;
				for (j = 0; j < i; j++)
					free(ret->params[j]);
				free(ret->params);

				ret->error = 10;
				return ret;
			}
			strcpy(ret->params[i], param);

			param = strtok(NULL, ",");
		}
		return ret;
		
	}
	case INST_TYPE_ENTRY:
	case INST_TYPE_EXTERN:
	{
		printf("malloc extern\n");
		if (!isValidSymbolName(line))
		{
			printf("malloc failed - not valid symbol name\n");
			/* Error: invalid data label */
			ret->error = 5;
			return ret;
		}
		
		/* copy label*/
		ret->params = (char**)malloc(sizeof(char*) + 1);
		if (!ret->params)
		{
			printf("malloc failed - params\n");
			/* malloc failed*/
			ret->error = 10;
			return ret;
		}

		printf("line 4 before malloc: %s\n", line);
		printf("line 4 before len: %d\n", strlen(line));
		ret->params[0] = malloc((sizeof(char*) * strlen(line)) + 1);/*ron added* */
		if (!ret->params[0])
		{
			printf("malloc failed - params[0]\n");
			/* malloc failed*/
			free(ret->params);
			ret->error = 10;
			return ret;
		}
		strcpy(ret->params[0], line);
		ret->numParams = 1;
		printf("ret prints %s \n", ret->params[0]);
		printf("ret prints %d\n", ret->error);
		
		
		printf("malloc succsess\n");
		return ret;
	}
	
	case INST_TYPE_STRING:
	{
		if (line[0] != '\"' || line[strlen(line) - 1] != '\"')
		{
			/* Error: invalid data string */
			ret->error = 5;
			return ret;
		}
		/* copy string from pos 1 to strlen-2*/
		/* remove "*/
		line++;
		line[strlen(line) - 1] = '\0';

		ret->params = (char**)malloc(sizeof(char*) * 1);
		if (!ret->params)
		{
			/* malloc failed*/
			ret->error = 10;
			return ret;
		}

		
		ret->params[0] = malloc((sizeof(char) * strlen(line))+ 1);
		if (!ret->params[0])
		{
			/* malloc failed*/
			free(ret->params);
			ret->error = 10;
			return ret;
		}
		strcpy(ret->params[0], line);
		ret->numParams = 1;

		return ret;
	}

	case INST_TYPE_INVALID:
	{
		ret->error = 3;
		return ret;
	}
	}




}

Operation* parseOperation(char* line)
{
	Operation* ret = (Operation*)malloc(sizeof(Operation));
	if (!ret)
		return NULL;

	ret->error = 0;
	ret->label[0] = '\0'; /* empty label*/
	ret->labelFlag = 0;
	ret->sourceType = NONE;
	ret->targetType = NONE;

	char *label = NULL,
		 *opcodeStr = NULL,
		 *operand1 = NULL,
		 *operand2 = NULL;


	line = removeLeadingSpaces(line);

	/* search for LABEL*/
	if (strchr(line, ':'))
	{
		label = strtok(line, ":");
		if (!isValidSymbolName(label))
		{
			/* Error: invalid label */
			ret->error = 2;
			return ret;
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
		ret->error = 3;
		return ret;
	}

	

	

	operand1 = strtok(line, ",");
	if (operand1 && !isWhiteSpacesLine(operand1))
	{

		operand1 = removeLeadingSpaces(operand1);
		removeTrailingSpaces(operand1);
		


		operand2 = strtok(NULL, "");
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

	if (ret->sourceType == INVALID_ADDR_METHOD || ret->targetType == INVALID_ADDR_METHOD)
	{
		ret->error = 1;
	}


	return ret;

}



void freeInstruction(Instruction* instruction)
{
	if (!instruction)
		return;

	int i;
	for (i = 0; i < instruction->numParams; i++)
	{
		free(instruction->params[i]);
	}
	free(instruction->params);
	free(instruction);
}

void freeOperation(Operation* operation)
{
	free(operation);
}