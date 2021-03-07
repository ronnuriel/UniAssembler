#include <string.h>
#include "Parser.h"
#include <string.h>
#include "HelperFunc.h"
#include "Symbol.h"
#include "Operator.h"
#include <stdio.h>

#pragma warning(disable: 4996)

LineTypeEnum detectLineType(char* line)
{
	if (!line)
	{
		return INVALID_LINE;
	}
	if (line[0] == '\0')//EMPTY_LINE - strlen = 0
	{
		return EMPTY_LINE;
	}
	if (line[0] == ';')//COMMENT_LINE
	{
		return COMMENT_LINE;
	}
	if (isWhiteSpacesLine(line)) //EMPTY_LINE - filled with whitespaces
	{
		return EMPTY_LINE;
	}
	if (strchr(line,'.'))//INSTRUCTION_LINE
	{
		return INSTRUCTION_LINE;
	}
	
	return OPERATION_LINE;
}


int parseIntruction(char* line)
{
}

Operation parseOperation(char* line)
{
	Operation ret;
	ret.label[0] = '\0'; // empty label
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
			printf("invalid symbol\n");
			//return 0;
		}
		strcpy(ret.label, label);
		/* prepeare line for next parsing */
		line = strtok(NULL, "");

		printf("label: %s.\n", label);

	}
	
	
	opcodeStr = strtok(line, " ");
	opcodeStr = removeLeadingSpaces(opcodeStr);
	removeTrailingSpaces(opcodeStr);

	line = strtok(NULL, "");
	ret.opcode = operatorStringToEnum(opcodeStr);
	if (ret.opcode == OPERATOR_INVALID)
	{
		/* Error: invalid operand */
		printf("invalid opcode\n");
		//return 0;
	}

	printf("opcode:%s.\n", opcodeStr);

	

	operand1 = strtok(line, ",");
	if (operand1 && !isWhiteSpacesLine(operand1))
	{

		operand1 = removeLeadingSpaces(operand1);
		removeTrailingSpaces(operand1);
		printf("1:%s.\n", operand1);

		

		operand2 = strtok(NULL, "");
		if (operand2 && !isWhiteSpacesLine(operand2))
		{
			operand2 = removeLeadingSpaces(operand2);
			removeTrailingSpaces(operand2);
			printf("2:%s.\n", operand2);

			strcpy(ret.source, operand1);
			strcpy(ret.target, operand2);
		}
		else
		{
			strcpy(ret.target, operand1);
		}
	}

	/* we have all parts of the line */


	return ret;

}
