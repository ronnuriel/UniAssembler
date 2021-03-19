#include "Compiler.h"
#include "File.h"
#include "Parser.h"
#include "SymbolList.h"
#include "CodeList.h"
#include "stdio.h"


#pragma warning(disable: 4996)


#define MAX_LINE_LENGTH 82 /* include \n and \0 */
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
	printf("1\n");

	if (!dataList || !operationList || !symbolList) 
	{
		freeCodeList(dataList);
		freeCodeList(operationList);
		freeSymbolList(symbolList);
		return -1;
	}

	if (!createFileNames(inputFilePath, &asFilePath, &obFilePath, &entFilePath, &extFilePath))
	{
		freeCodeList(dataList);
		freeCodeList(operationList);
		freeSymbolList(symbolList);
		return -1;
	}
	if(!openInputFile(asFilePath))
	{
		freeCodeList(dataList);
		freeCodeList(operationList);
		freeSymbolList(symbolList);
		return -1;
	}	

	/* First pass */
	printf("1\n");
	char line[MAX_LINE_LENGTH];
	while (readNextLine(line, MAX_LINE_LENGTH))
	{
		LineTypeEnum lineType = detectLineType(line);
		printf("line: %s\n", line);
		printf("linetype: %d\n" ,lineType);

		switch (lineType)
		{
		case INVALID_LINE:
		{
			printf("INVALID LINE!!!!!!\n\n");
			break;

		}
		case EMPTY_LINE:
		case COMMENT_LINE:
		{
			continue;
		}
		case INSTRUCTION_LINE:
		{
			printf("Instruction\n");
			compileInstruction(line, symbolList, dataList);
			
			break;
		}
		case OPERATION_LINE:
		{
			compileOperation(line, symbolList, operationList);
			
				
			/*
			
			int isSourceAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);
			//int isDestAddrMethodLegitByOperator(OperatorsEnum op, AddrMethodEnum method);
			
			//compileOperation(line, symbolList, )
				*/
			break;
		}
		}
	}

	

	printSymbolList(symbolList);
	printf("data list:\n");
	printCodeList(dataList);
	printf("operation list:\n");
	printCodeList(operationList);

	/* update addresses of DATA labels in symbol list*/
	updateSymbolListValuesOfData(symbolList, operationList->currAddr);
	
	/* second pass */
	rewindInputFile();

	/* find all entries*/
	updateEntries(symbolList);

	printf("symbol list after update:\n");
	printSymbolList(symbolList);


	closeInputFile();

	updateRelativeAndDirectLabelsInCodeList(operationList, symbolList);

	printf("operation list after update:\n");
	printCodeList(operationList);

	/* generate object file */
	generateObjectFile(operationList, dataList, obFilePath);
	
	/* generate entry file */
	generateAttributeFile(symbolList, entFilePath, ENTRY);

	/* generate external file*/
	generateExternalFile(operationList, extFilePath);
	free(asFilePath);
	free(obFilePath);
	free(extFilePath);
	free(entFilePath);

}
int compileInstruction(char *line, SymbolList* symbolList, CodeList* dataList)
{
	

	Instruction* instruction = parseIntruction(line);

	printf("0instructin11: %d", instruction->type);
	printf("0instructin: %p", instruction);
	printf("0params: %p", instruction->params);
	switch (instruction->type)
	{
	
	case INST_TYPE_ENTRY:
	{
		return 1;
	}
	case INST_TYPE_EXTERN:
	{
		printf("26Extern11\n");
		printf("26Extern11\n");
		printf("26Extern11\n");
		/*printf("26instructin11: %d", instruction->type);*/
		printf("26 params: %s ",(instruction->params)[0]);
		if (getSymbolRowByName(symbolList, instruction->params[0]))
		{
			printf("error\n");
			/* label already defined!. error*/
			freeInstruction(instruction);
			return 0;
		}
		printf("000\n");
		addSymbolToList(symbolList, instruction->params[0], 0, EXTERNAL);
		printf("after\n");
		break;
	}
	case INST_TYPE_STRING:
	case INST_TYPE_DATA:
	{
		if (instruction->labelFlag == 1)
		{
			/* string instruction with label. */
			if (getSymbolRowByName(symbolList, instruction->params[0]))
			{
				/*label already defined!. error*/
				freeInstruction(instruction);
				return 0;
			}

			addSymbolToList(symbolList, instruction->label, getCodeListCurrentAddr(dataList), DATA);/*11??IC*/
		}
		
		/*hande data and increment DC*/
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
	printf("before free\n");
	freeInstruction(instruction);
	return 1;
}

int compileOperation(char* line, SymbolList* symbolList, CodeList* operationList)
{

	Operation* operation = parseOperation(line);
	if (operation->labelFlag == 1)
	{
		/* operation with label.*/ 
		if (getSymbolRowByName(symbolList, operation->label))
		{
			/*label already defined!. error*/
			return 0;
		}
		addSymbolToList(symbolList, operation->label, getCodeListCurrentAddr(operationList), CODE); /* line 11*/
	}

	addOperationToCodeList(operationList, operation);

	freeOperation(operation);
	return 1;
}

void updateEntries(SymbolList* symbolList)
{
	char line[MAX_LINE_LENGTH];
	while (readNextLine(line, MAX_LINE_LENGTH))
	{
		LineTypeEnum lineType = detectLineType(line);

		if (lineType != INSTRUCTION_LINE)
			continue;

		/* line is instruction*/
		Instruction* instruction = parseIntruction(line);
		if (instruction->type == INST_TYPE_ENTRY)
		{
			/* found entry! */
			if (!addAttributeToSymbolInSymbolList(symbolList, instruction->params[0], ENTRY))
			{
				printf("Error symbol not found!");
			}

		}

		freeInstruction(instruction);
	}
}

int generateObjectFile(CodeList* operationList, CodeList* dataList, char *path)
{
	if (!openOutputFile(path))
		return 0;

	/* write caption - length of operation list and data list*/
	char caption[MAX_OBJECT_FILE_CAPTION_LEN];
	sprintf(caption, "%d %d\n", getCodeListLength(operationList), getCodeListLength(dataList));
	writeOutput(caption);

	/* write operation list*/
	printCodeListToFunc(operationList, 0, writeOutput);
	
	/* write data list*/
	printCodeListToFunc(dataList, operationList->currAddr, writeOutput);
	closeOutputFile();
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