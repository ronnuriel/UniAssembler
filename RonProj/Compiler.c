

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

	CodeList* dataList = initCodeList(DC_START_POS);
	CodeList* operationList = initCodeList(IC_START_POS);//ron
	if (!dataList || !operationList) // ron
	{
		freeCodeList(dataList);
		freeCodeList(operationList);
		return -1;
	}

	if (!createFileNames(inputFilePath, &asFilePath, &obFilePath, &entFilePath, &extFilePath))
	{
		freeCodeList(dataList);
		freeCodeList(operationList);
		return -1;
	}
	if(!openInputFile(asFilePath))
	{
		freeCodeList(dataList);
		freeCodeList(operationList);
		return -1;
	}	

	/* Work on parsing */
	/* First pass */

	

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
			compileInstruction(line, symbolList, dataList);
			//TODO: instruction
			break;
		}
		case OPERATION_LINE:
		{
			Operation* operation = parseOperation(line);
			if (operation->label[0] != '\0')
				

			//int isSourceAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);
			//int isDestAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);
			
			//compileOperation(line, symbolList, )
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
int compileInstruction(char *line, SymbolList* symbolList, CodeList* dataList)
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
			freeInstruction(instruction);
			return 0;
		}

		addSymbolToList(symbolList, instruction->params[0], 0, EXTERNAL);
		
		break;
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
				freeInstruction(instruction);
				return 0;
			}
			addSymbolToList(symbolList, instruction->label, getCodeListCurrentAddr(dataList), DATA);//11??IC
		}

		// hande data and increment DC
		if (instruction->type == INST_TYPE_STRING)
		{
			addStringToCodeList(dataList, instruction->params[0]);
		}
		else
		{
			addDataToCodeList(dataList, instruction->params, instruction->numParams);
		}
		
		break;
	}
	}
	freeInstruction(instruction);
	return 1;
}

int compileOperation(char* line, SymbolList* symbolList, CodeList* operationList)
{

	Operation* operation = parseOperation(line);
	if (operation->labelFlag == 1)
	{
		// operation with label. 
		if (getSymbolRowByName(symbolList, operation->label))
		{
			// label already defined!. error
			return 0;
		}
		addSymbolToList(symbolList, operation->label, getCodeListCurrentAddr(operationList), CODE); // line 11
	}


	freeOperation(operation);
	return 1;
}