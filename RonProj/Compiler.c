

#include "Compiler.h"
#include "File.h"
#include "Parser.h"

#define MAX_LINE_LENGTH 82 /* include \n and \0 */
#define DC_START_POS 0
#define IC_START_POS 100
int compileFile(char* inputFilePath)
{
	char* asFilePath = NULL,
		* obFilePath = NULL,
		* extFilePath = NULL,
		* entFilePath = NULL;

	if (!createFileNames(inputFilePath, &asFilePath, &obFilePath, &entFilePath, &extFilePath))
	{
		return -1;
	}
	if(!openInputFile(asFilePath))
	{
		return -1;
	}	

	/* Work on parsing */
	/* First pass */
	int DC = DC_START_POS, IC = IC_START_POS;

	char line[MAX_LINE_LENGTH];
	while (!readNextLine(line, MAX_LINE_LENGTH))
	{
		LineTypeEnum lineType = detectLineType(line);
		Instruction instruction;

		switch (lineType)
		{
		case INVALID_LINE:
		{

			break;

		}
		case EMPTY_LINE:
		case COMMENT_LINE:
		{
			continue;
		}
		case INSTRUCTION_LINE:
		{
			parseIntruction(line);
			//TODO: instruction
			break;
		}
		case OPERATION_LINE:
		{
			parseOperation(line);
			//int isSourceAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);
			//int isDestAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);
			break;
		}
		}
	}

	closeInputFile();
	free(asFilePath);
	free(obFilePath);
	free(extFilePath);
	free(entFilePath);

}