#include "CodeList.h"
#include <stdlib.h>
#include <string.h>

#include "Operator.h"
#include "Register.h"

#include "File.h"
#include "stdio.h"

#pragma warning(disable: 4996)
CodeListRow* createCodeListRow(int address, unsigned int word, char ARE, char* data, int lineNum)/*creat row*/
{

	CodeListRow* ret = (CodeListRow*)malloc(sizeof(CodeListRow));
	if (!ret)
		return NULL;
	ret->lineNum = lineNum;
	ret->address = address;
	ret->word = word;
	ret->ARE = ARE;
	ret->data[0] = '\0';
	
	/* copy data if present*/
	if (data)
	{
		strcpy(ret->data, data);
	}

	return ret;
}
void freeCodeListRow(CodeListRow* row) /*free row from list*/
{
	printCodeListRow(row);
	//free(row->data);
	free(row);
}
CodeList* initCodeList(int startAddr) /* initialize*/
{

	CodeList* ret = (CodeList*)malloc(sizeof(CodeList));
	if (!ret)
		return NULL;

	ret->list = initList();
	if (!ret->list)
	{
		free(ret);
		return NULL;
	}

	ret->currAddr = startAddr;
	return ret;
}
int getCodeListLength(CodeList* clist)
{
	return clist->list->length;
}
int addCodeToList(CodeList* clist, unsigned int word, char ARE, char* data, int lineNum)/*add code */
{
	if (!clist)
	{
		return 0;
	}

	CodeListRow* newRow = createCodeListRow(clist->currAddr, word, ARE, data, lineNum);
	if (!newRow)
	{
		return 0;
	}
	if (!addToList((void*)newRow, clist->list))
	{
		free(newRow);
		return 0;
	}

	(clist->currAddr)++;
	return 1;
}

void freeCodeList(CodeList* clist) /*free the list using the func freeCodeListRow */
{
	if (!clist)
		return;

	freeList(clist->list, freeCodeListRow);
	free(clist);
}
int getCodeListCurrentAddr(CodeList* clist)/*get func */
{
	return clist->currAddr;
}

int addStringToCodeList(CodeList* clist, char* str, int lineNum)/*adds the string into */
{

	unsigned int i;
	for (i = 0; i < strlen(str)+1; i++) /* +1 to include \0*/
	{
		if (!addCodeToList(clist, str[i], ARE_A, NULL, lineNum))
		{
			printf("Error: Allocation failed while adding string %s to data\n", str);
			return 0;
		}
	}

	return 1;
}
int addDataToCodeList(CodeList* clist, char** params, int numParams, int lineNum)
{
	int i;
	for (i = 0; i < numParams; i++) 
	{
		char* paramStr = params[i];

		if (!addCodeToList(clist, atoi(paramStr), ARE_A, NULL, lineNum))
		{
			printf("Error: Allocation failed while adding value %s to data\n", paramStr);
			return 0;
		}
	}
	return 1;
}

int addOperationToCodeList(CodeList* clist, Operation* op, int lineNum)
{
	if (!addCodeToList(clist, generateBinaryWord(op->opcode, op->sourceType, op->targetType), ARE_A, NULL, lineNum))
	{
		printf("Error: Allocation failed while adding operator to list\n");
		return 0;
	}
	if (!addOperandToCodeList(clist, op->sourceType, op->source, lineNum))
	{
		printf("Error: Allocation failed while adding operand to list\n");
		return 0;
	}
	if (!addOperandToCodeList(clist, op->targetType, op->target, lineNum))
	{
		printf("Error: Allocation failed while adding operand to list\n");
		return 0;
	}

	return 1;
}

int addOperandToCodeList(CodeList* clist, AddrMethodEnum type, char* value, int lineNum)
{
	int valueNum = atoi(value);
	switch (type)
	{
	case NONE:
	{
		return 1;
	}
	case IMMEDIATE:
	{
		return addCodeToList(clist, (unsigned int)valueNum, ARE_A, NULL, lineNum);
	}
	case RELATIVE:
	{
		return addCodeToList(clist, 0, ARE_RELATIVE, value, lineNum);
	}
	case DIRECT:
	{
		return addCodeToList(clist, 0, ARE_DIRECT, value, lineNum);
	}
	case REGISTER_DIRECT:
	{
		return addCodeToList(clist, RegisterNameToBinary(value), ARE_A, NULL, lineNum);
	}
	default:
		return 1;
	}
}

void printCodeListRow(CodeListRow* row)
{
	unsigned int twelveBits = row->word & 0xFFF;
	printf("Address: %d  word: %x ARE: %c line: %d", row->address, twelveBits, row->ARE, row->lineNum);
	if (row->data[0] != '\0')
		printf(" data: %s", row->data);
	
	printf("\n");

}

void printCodeList(CodeList* clist)
{
	

	if (!clist)
	{
		printf("No code list\n");
		return;
	}
	else
	{
		int len = clist->list->length;
		Node* t = clist->list->head;

		printf("Code List: length %d. next available address: %d\n", len, clist->currAddr);
		printf("================================\n");
		

		while (t)
		{
			CodeListRow* row = t->data;
			printCodeListRow(row);
			t = getNodeNext(t);
		}

		printf("\n=== End of code list ====\n\n");
	}
}

int updateRelativeAndDirectLabelsInCodeList(CodeList* clist, SymbolList* symbolList)
{
	Node* t = clist->list->head;
	int retVal = 1;
	while (t)
	{
		CodeListRow* row = t->data;
		
		if (row->ARE == ARE_DIRECT || row->ARE == ARE_RELATIVE)
		{
			SymbolListRow* symbolRow = getSymbolRowByName(symbolList, row->data);
			if (!symbolRow)
			{
				printf("Error: could not find symbol %s in line %d in symbol-list\n", row->data, row->lineNum);
				retVal = 0;
			}
			else
			{
				switch (row->ARE)
				{
				case ARE_DIRECT:
				{
					row->word = (unsigned int)symbolRow->value;
					break;
				}
				case ARE_RELATIVE:
				{
					row->word = (unsigned int)(symbolRow->value - row->address);
					break;
				}
				}

				row->ARE = isRowOfType(symbolRow, EXTERNAL) ? ARE_E : ARE_R;
			}
		}
		
		t = getNodeNext(t);
	}
	return retVal;
}


int printCodeListToFunc(CodeList* clist, int offset, int func(char* str))
{
	if (!clist)
	{
		return 1;
	}
	else
	{
		Node* t = clist->list->head;

		while (t)
		{
			CodeListRow* row = t->data;
			char str[FILE_ADDR_LEN + 1 + FILE_VALUE_LEN + 1 + 1 + 1 + 1]; /* 0000 000 A\n\0*/
			sprintf(str, "%.4d %.3X %c\n", row->address + offset, row->word & 0xFFF, row->ARE);
			writeOutput(str);
			t = getNodeNext(t);
		}

		return 1;
	}
}
int printCodeListExternalsToFunc(CodeList* clist, int func(char* str))
{
	if (!clist)
	{
		return 1;
	}
	else
	{
		Node* t = clist->list->head;

		while (t)
		{
			CodeListRow* row = t->data;
			if (row->ARE == ARE_E)
			{
				char value[FILE_ADDR_LEN + 1 + 1];
				sprintf(value, "%.4d\n", row->address);

				writeOutput(row->data);
				writeOutput(" ");
				writeOutput(value);
			}
			t = getNodeNext(t);
		}

		return 1;
	}
}

int doesCodeListIncludeExternals(CodeList* clist)
{
	if (!clist)
	{
		return 0;
	}
	else
	{
		Node* t = clist->list->head;

		while (t)
		{
			CodeListRow* row = t->data;
			if (row->ARE == ARE_E)
			{
				return 1;
			}
			t = getNodeNext(t);
		}

		return 0;
	}
}