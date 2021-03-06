

#include "Compiler.h"
#include "File.h"
#include "Parser.h"

#define MAX_LINE_LENGTH 82 /* include \n and \0 */

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
	char line[MAX_LINE_LENGTH];
	while (!readNextLine(line, MAX_LINE_LENGTH))
	{
		LineTypeEnum lineType = detectLineType(line);
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
			break;
		}
		case OPERATION_LINE:
		{
			parseOperation(line);
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