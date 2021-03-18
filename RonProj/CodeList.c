#include "CodeList.h"
#include <stdlib.h>
#include <string.h>

#include "Operator.h"
#include "Register.h"

#include "File.h"
#include "stdio.h"

#pragma warning(disable: 4996)
CodeListRow* createCodeListRow(int address, unsigned int word, char ARE, char* data)/*creat row*/
{

	CodeListRow* ret = (CodeListRow*)malloc(sizeof(CodeListRow));
	if (!ret)
		return NULL;
	
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
	free(row->data);
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
}
int getCodeListLength(CodeList* clist)
{
	return clist->list->length;
}
void addCodeToList(CodeList* clist, unsigned int word, char ARE, char* data)/*add code */
{
	if (!clist)
	{
		return;
	}
	addToList((void*)createCodeListRow(clist->currAddr, word, ARE, data), clist->list);
	(clist->currAddr)++;
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

void addStringToCodeList(CodeList* clist, char* str)/*adds the string into */
{
	for (int i = 0; i < strlen(str)+1; i++) // +1 to include \0
	{
		addCodeToList(clist, str[i], ARE_A, NULL);
	}
}
void addDataToCodeList(CodeList* clist, char** params, int numParams)
{
	for (int i = 0; i < numParams; i++) 
	{
		char* paramStr = params[i];

		addCodeToList(clist, atoi(paramStr), ARE_A, NULL);
	}
}

void addOperationToCodeList(CodeList* clist, Operation* op)
{
	addCodeToList(clist, generateBinaryWord(op->opcode, op->sourceType, op->targetType), ARE_A, NULL);
	addOperandToCodeList(clist, op->sourceType, op->source);
	addOperandToCodeList(clist, op->targetType, op->target);
}

void addOperandToCodeList(CodeList* clist, AddrMethodEnum type, char* value)
{
	int valueNum = atoi(value);
	switch (type)
	{
	case NONE:
	{
		break;
	}
	case IMMEDIATE:
	{
		addCodeToList(clist, (unsigned int)valueNum, ARE_A, NULL);
		break;
	}
	case RELATIVE:
	{
		addCodeToList(clist, 0, ARE_RELATIVE, value);
		break;
	}
	case DIRECT:
	{
		addCodeToList(clist, 0, ARE_DIRECT, value); 
		break;
	}
	case REGISTER_DIRECT:
	{
		addCodeToList(clist, RegisterNameToBinary(value), ARE_A, NULL);
		break;
	}

	}
}

void printCodeListRow(CodeListRow* row)
{
	unsigned int twelveBits = row->word & 0xFFF;
	printf("Address: %d  word: %x ARE: %c", row->address, twelveBits, row->ARE);
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
	
	int len = clist->list->length;
	printf("Code List: length %d. next available address: %d\n", len, clist->currAddr);
	printf("================================\n");
	Node* t = clist->list->head;

	while (t)
	{
		CodeListRow* row = t->data;
		printCodeListRow(row);
		t = getNodeNext(t);
	}

	printf("\n=== End of code list ====\n\n");
}

int updateRelativeAndDirectLabelsInCodeList(CodeList* clist, SymbolList* symbolList)
{
	Node* t = clist->list->head;

	while (t)
	{
		CodeListRow* row = t->data;
		int value;
		if (row->ARE == ARE_DIRECT || row->ARE == ARE_RELATIVE)
		{
			SymbolListRow* symbolRow = getSymbolRowByName(symbolList, row->data);
			if (!symbolRow)
			{
				/* label not found! ERROR */
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
	return 1;
}


int printCodeListToFunc(CodeList* clist, int offset, int func(char* str))
{
	if (!clist)
	{
		return 1;
	}
	Node* t = clist->list->head;

	while (t)
	{
		CodeListRow* row = t->data;
		char str[FILE_ADDR_LEN + 1 +FILE_VALUE_LEN + 1 + 1 + 1 + 1]; /* 0000 000 A\n\0*/
		sprintf(str, "%.4d %.3X %c\n", row->address + offset, row->word & 0xFFF, row->ARE);
		writeOutput(str);
		t = getNodeNext(t);
	}

	return 1;
}
int printCodeListExternalsToFunc(CodeList* clist, int func(char* str))
{
	if (!clist)
	{
		return 1;
	}
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

int doesCodeListIncludeExternals(CodeList* clist)
{
	if (!clist)
	{
		return 0;
	}
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