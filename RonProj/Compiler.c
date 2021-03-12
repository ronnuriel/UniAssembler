

#include "Compiler.h"
#include "File.h"
#include "Parser.h"
#include "SymbolList.h"
#include "CodeList.h"

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
	CodeList* dataList = initCodeList();
	if (!dataList)
	{
		return -1;
	}
	SymbolList* symbolList = initSymbolList();


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
			
			//TODO: instruction
			break;
		}
		case OPERATION_LINE:
		{
			Operation* operation = parseOperation(line);
			if (operation->label[0] != '\0')
				

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
int compileInstruction(char *line, int* DC, int* IC, SymbolList* symbolList)
{
	

	Instruction* instruction = parseIntruction(line);


	switch (instruction->type)
	{
	
	case INST_TYPE_ENTRY:
	{
		return 1;
	}
	case INST_TYPE_EXTERN:
	{
		if (getSymbolRowByName(symbolList, instruction->params[0]))
		{
			// label already defined!. error
			return 0;
		}

		addSymbolToList(symbolList, instruction->params[0], 0, EXTERNAL);
		return 1;
	}
	case INST_TYPE_STRING:
	case INST_TYPE_DATA:
	{
		if (instruction->labelFlag == 1)
		{
			// string instruction with label. 
			if (getSymbolRowByName(symbolList, instruction->params[0]))
			{
				// label already defined!. error
				return 0;
			}
			addSymbolToList(symbolList, instruction->label, *DC, CODE);
		}
		// hande data and increment DC
	}
	}
}