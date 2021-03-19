

#include "Compiler.h"
#include "File.h"
#include "Parser.h"
#include "SymbolList.h"
#include "CodeList.h"

#include "stdio.h"


#pragma warning(disable: 4996)


#define DC_START_POS 0
#define IC_START_POS 100
#define MAX_OBJECT_FILE_CAPTION_LEN 50
int compileFile(char* inputFilePath)
{

	char* asFilePath = NULL,
		* obFilePath = NULL,
		* extFilePath = NULL,
		* entFilePath = NULL;

	CodeList* dataList = initCodeList(DC_START_POS);
	CodeList* operationList = initCodeList(IC_START_POS);
	SymbolList* symbolList = initSymbolList();

	char line[MAX_LINE_LENGTH + LINE_EXTRA_LENGTH];
	int retFirstPass = 1, retSecondPass = 1;
	int lineNum = 0;
	FILE_STATUS lineStatus;
	if (!dataList || !operationList || !symbolList)
	{
		freeCodeList(dataList);
		freeCodeList(operationList);
		freeSymbolList(symbolList);
		printf("Error: Allocation failed during list initialization\n");
		return 0;
	}

	if (!createFileNames(inputFilePath, &asFilePath, &obFilePath, &entFilePath, &extFilePath))
	{
		freeCodeList(dataList);
		freeCodeList(operationList);
		freeSymbolList(symbolList);
		printf("Error: Allocation failed during name string inialization\n");
		return 0;
	}
	if (!openInputFile(asFilePath))
	{
		freeCodeList(dataList);
		freeCodeList(operationList);
		freeSymbolList(symbolList);
		printf("Error: Input file could not be opened\n");
		return 0;
	}

	/* First pass */
	lineNum = 0;
	lineStatus = readNextLine(line, MAX_LINE_LENGTH);
	while (lineStatus)
	{
		lineNum++;
		
		if (lineStatus == LINE_TOO_LONG)
		{
			printf("Error: Line %d too long\n", lineNum);
		}
		else
		{
			
			LineTypeEnum lineType = detectLineType(line);

			switch (lineType)
			{
			case INVALID_LINE:
			{
				printf("Error: Invalid line at line %d: %s\n", lineNum, line);
				break;
			}
			case EMPTY_LINE:
			case COMMENT_LINE:
			{
				break;
			}
			case INSTRUCTION_LINE:
			{

				if (!compileInstruction(line, symbolList, dataList, lineNum))
				{
					retFirstPass = 0;
				}

				break;
			}
			case OPERATION_LINE:
			{
				if (!compileOperation(line, symbolList, operationList, lineNum))
				{
					retFirstPass = 0;
				}

				break;
			}
			}
		}
		lineStatus = readNextLine(line, MAX_LINE_LENGTH);
	}

	/* finished first pass*/

	/*
	printSymbolList(symbolList);
	printf("data list:\n");
	printCodeList(dataList);
	printf("operation list:\n");
	printCodeList(operationList);
	*/


	/* second pass only if no error*/
	if (retFirstPass)
	{
		/* update addresses of DATA labels in symbol list*/
		updateSymbolListValuesOfData(symbolList, operationList->currAddr);
		
		rewindInputFile();

		/* find all entries*/
		if (!updateEntries(symbolList))
			retSecondPass = 0;

		/*printf("symbol list after update:\n");
		printSymbolList(symbolList);
		*/

		

		if (!updateRelativeAndDirectLabelsInCodeList(operationList, symbolList))
			retSecondPass = 0;

		/*printf("operation list after update:\n");
		printCodeList(operationList);
		*/
		if (retSecondPass)
		{
			/* generate object file */
			if (!generateObjectFile(operationList, dataList, obFilePath))
			{
				printf("Error: could not generate object file\n");
				retSecondPass = 0;
			}
			/* generate entry file */
			if (!generateAttributeFile(symbolList, entFilePath, ENTRY))
			{ 
				printf("Error: could not generate entries file\n");
				retSecondPass = 0;
			}
			/* generate external file*/
			if (!generateExternalFile(operationList, extFilePath))
			{ 
				printf("Error: could not generate externals file\n");
				retSecondPass = 0;
			}
		}
	}

	closeInputFile();

	/* free everything */
	free(asFilePath);
	free(obFilePath);
	free(extFilePath);
	free(entFilePath);

	freeSymbolList(symbolList);
	freeCodeList(operationList);
	freeCodeList(dataList);

	return retFirstPass && retSecondPass;
}


int compileInstruction(char *line, SymbolList* symbolList, CodeList* dataList, int lineNum)
{
	
	Instruction* instruction = parseIntruction(line);
	int retVal = 1;
	if (!instruction)
	{
		printf("Error: Allocation failed while parsing instruction in line %d\n", lineNum);
		return 0;
	}

	/* check parsing errors*/
	if (instruction->error & PE_INVALID_SYM_NAME)
	{
		printf("Error: Invalid symbol name in line %d\n", lineNum);
		retVal = 0;
	}
	if (instruction->error & PE_COMMA_AT_START_OR_END)
	{
		printf("Error: Extra commas at start or end at line: %d\n", lineNum);
		retVal = 0;
	}
	else if (instruction->error & PE_PARAM_NOT_A_NUM)
	{
		printf("Error: Parameter is not a number at line: %d\n", lineNum);
		retVal = 0;
	}
	else if (instruction->error & PE_PARAM_NOT_A_VALID_SYM)
	{
		printf("Error: Parameter is not a valid label at line: %d\n", lineNum);
		retVal = 0;
	}
	else if (instruction->error & PE_PARAM_NOT_A_STR)
	{
		printf("Error: Parameter is not a valid string at line: %d\n", lineNum);
		retVal = 0;
	}
	else if (instruction->error & PE_INVALID_INSTRUCTION)
	{
		printf("Error: Instruction unkown at line: %d\n", lineNum);
		retVal = 0;
	}
	else if (instruction->error & PE_MISSING_PARAMS)
	{
		printf("Error: Missing parameters at line: %d\n", lineNum);
		retVal = 0;
	}
	if (retVal != 0)
	{
		switch (instruction->type)
		{

		case INST_TYPE_ENTRY:
		{
			break;
		}
		case INST_TYPE_EXTERN:
		{
			if (getSymbolRowByName(symbolList, instruction->params[0]))
			{
				/* label already defined!. error*/
				printf("Error at line %d: Label: %s already defined\n", lineNum, instruction->params[0]);
				freeInstruction(instruction);
				return 0;
			}

			if (!addSymbolToList(symbolList, instruction->params[0], 0, EXTERNAL))
			{
				freeInstruction(instruction);
				return 0;
			}

			break;
		}
		case INST_TYPE_STRING:
		case INST_TYPE_DATA:
		{
			if (instruction->labelFlag == 1)
			{
				/* string instruction with label. */
				if (getSymbolRowByName(symbolList, instruction->label))
				{
					/*label already defined!. error*/
					printf("Error at line %d: Label: %s already defined\n", lineNum, instruction->label);
					freeInstruction(instruction);
					return 0;
				}

				if (!addSymbolToList(symbolList, instruction->label, getCodeListCurrentAddr(dataList), DATA))
				{
					freeInstruction(instruction);
					return 0;
				}
			}


			/*hande data and increment DC*/
			if (!instruction->error && instruction->type == INST_TYPE_STRING)
			{
				if (!addStringToCodeList(dataList, instruction->params[0], lineNum))
				{
					freeInstruction(instruction);
					return 0;
				}
			}
			else if (!instruction->error && !addDataToCodeList(dataList, instruction->params, instruction->numParams, lineNum))
			{
				freeInstruction(instruction);
				return 0;
			}

			break;
		}
		default:
			break;
		}
	}
	freeInstruction(instruction);
	return retVal;
}

int compileOperation(char* line, SymbolList* symbolList, CodeList* operationList, int lineNum)
{
	int retVal = 1;
	Operation* operation = parseOperation(line);
	if (!operation)
	{
		printf("Error: Allocation failed while parsing operation in line %d\n", lineNum);
		return 0;
	}

	/* check parsing errors*/

	if (operation->error & PE_INVALID_SYM_NAME)
	{
		printf("Error: Invalid symbol name in line %d\n", lineNum);
		retVal = 0;
	}
	else if (operation->error & PE_INVALID_OPERATOR)
	{
		printf("Error: Invalid operator in line %d\n", lineNum);
		retVal = 0;
	}
	else if (operation->error & PE_INVALID_SOURCE_OPERAND)
	{
		printf("Error: Invalid source operand in line %d\n", lineNum);
		retVal = 0;
	}
	else if (operation->error & PE_INVALID_TARGET_OPERAND)
	{
		printf("Error: Invalid target operand in line %d\n", lineNum);
		retVal = 0;
	}
	else if (operation->error & PE_TOO_MANY_OPERANDS)
	{
		printf("Error: More than 3 operands in line %d\n", lineNum);
		retVal = 0;
	}

	if (!isSourceAddrMethodLegitByOperator(operation->opcode, operation->sourceType))
	{
		if (operation->sourceType == NONE)
		{
			printf("Error: Missing source operand at line %d\n", lineNum);
			retVal = 0;
		}
		else if (operation->sourceType != INVALID_ADDR_METHOD)
		{
			/* already took care of  invalid source operand*/
			printf("Error: Source operand not comaptible with operator at line %d\n", lineNum);
			retVal = 0;
		}
	}
	if (!isDestAddrMethodLegitByOperator(operation->opcode, operation->targetType))
	{
		if (operation->targetType == NONE)
		{
			printf("Error: Missing destination operand at line %d\n", lineNum);
			retVal = 0;
		}
		else if (operation->targetType != INVALID_ADDR_METHOD)
		{
			/* already took care of  invalid source operand*/
			printf("Error: Destination operand not comaptible with operator at line %d\n", lineNum);
			retVal = 0;
		}
	}

	if (retVal == 0)
	{
		freeOperation(operation);
		return 0;
	}

	if (operation->labelFlag == 1)
	{
		/* operation with label.*/ 
		if (getSymbolRowByName(symbolList, operation->label))
		{
			/*label already defined!. error*/
			printf("Error at line %d: Label: %s already defined\n", lineNum, operation->label);
			freeOperation(operation);
			return 0;
		}
		if (!addSymbolToList(symbolList, operation->label, getCodeListCurrentAddr(operationList), CODE))
		{
			freeOperation(operation);
			return 0;
		}
	}
				
	if (!addOperationToCodeList(operationList, operation, lineNum))
	{
		freeOperation(operation);
		return 0;
	}
	else
	{
		freeOperation(operation);
		return 1;
	}
}

int updateEntries(SymbolList* symbolList)
{
	char line[MAX_LINE_LENGTH];
	int lineNum = 1;
	int retVal = 1;
	while (readNextLine(line, MAX_LINE_LENGTH))
	{
		LineTypeEnum lineType = detectLineType(line);

		if (lineType != INSTRUCTION_LINE)
		{
			continue;
		}
		else
		{
			/* line is instruction*/
			Instruction* instruction = parseIntruction(line);
			if (instruction->type == INST_TYPE_ENTRY)
			{
				/* found entry! */
				if (!addAttributeToSymbolInSymbolList(symbolList, instruction->params[0], ENTRY))
				{

					printf("Error: Symbol %s declared as entry in line: %d was not found in symbol-list\n", instruction->params[0], lineNum);
					retVal = 0;
				}
			}

			freeInstruction(instruction);
			lineNum++;
		}
	}
	return retVal;
}

int generateObjectFile(CodeList* operationList, CodeList* dataList, char *path)
{
	if (!openOutputFile(path))
	{
		return 0;
	}
	else
	{
		/* write caption - length of operation list and data list*/
		char caption[MAX_OBJECT_FILE_CAPTION_LEN];
		sprintf(caption, "%d %d\n", getCodeListLength(operationList), getCodeListLength(dataList));
		writeOutput(caption);

		/* write operation list*/
		printCodeListToFunc(operationList, 0, writeOutput);

		/* write data list*/
		printCodeListToFunc(dataList, operationList->currAddr, writeOutput);
		closeOutputFile();

		return 1;
	}
}

int generateAttributeFile(SymbolList* symbolList, char* path, SymbolAttributesEnum attribute)
{

	if (doesSymbolListIncludeAttribute(symbolList, attribute))
	{
		if (!openOutputFile(path))
			return 0;
		printSymbolListToFuncByAttribute(symbolList, attribute, writeOutput);
		closeOutputFile();
	}
	return 1;
}

int generateExternalFile(CodeList* operationList, char* path)
{
	if (doesCodeListIncludeExternals(operationList))
	{
		if (!openOutputFile(path))
			return 0;
		printCodeListExternalsToFunc(operationList, writeOutput);
		closeOutputFile();
	}

	return 1;
}