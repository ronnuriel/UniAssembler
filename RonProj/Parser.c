#include <string.h>
#include "Parser.h"
#include <string.h>
#include "HelperFunc.h"
#include "Symbol.h"

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
int parseOperation(char* line)
{
	char *label = NULL,
		 *opcode = NULL,
		 *operand1 = NULL,
		 *operand2 = NULL;

	/* search for LABEL*/
	if (strchr(line, ":"))
	{
		label = strtok(line, ":");
		if (!isValidSymbolName(label))
		{
			/* Error: invalid label */
			printf("invalid symbol\n");
		}

		line = strtok(NULL, "");



	}

	line = removeLeadingSpaces(line);

}