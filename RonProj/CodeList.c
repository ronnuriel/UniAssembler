#include "CodeList.h"
#include <stdlib.h>

#include "Operator.h"
#include "Register.h"


#include "stdio.h"

CodeListRow* createCodeListRow(int address, unsigned int word, char ARE)/*creat row*/
{

	CodeListRow* ret = (CodeListRow*)malloc(sizeof(CodeListRow));
	if (!ret)
		return NULL;
	ret->address = address;
	ret->word = word;
	ret->ARE = ARE;

	return ret;

}
void freeCodeListRow(CodeListRow* row) /*free row from list*/
{
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

void addCodeToList(CodeList* clist, unsigned int word, char ARE)/*add code */
{
	if (!clist)
	{
		return;
	}
	addToList((void*)createCodeListRow(clist->currAddr, word, ARE), clist->list);
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
		addCodeToList(clist, str[i], 'A');
	}
}
void addDataToCodeList(CodeList* clist, char** params, int numParams)
{
	for (int i = 0; i < numParams; i++) 
	{
		char* paramStr = params[i];

		addCodeToList(clist, atoi(paramStr), 'A');
	}
}

void addOperationToCodeList(CodeList* clist, Operation* op)
{
	addCodeToList(clist, generateBinaryWord(op->opcode, op->sourceType, op->targetType), 'A');
	//addMethodAddrToCodeList(clist, op->sourceType, op->source);
	//addMethodAddrToCodeList(clist, op->targetType, op->target);
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
	case RELATIVE:
	{
		break;
		
		addCodeToList(clist, (unsigned int)valueNum, 'A');
	}
	case DIRECT:
	{
		addCodeToList(clist, 0, '?'); 
		break;
	}

	case REGISTER_DIRECT:
	{
		addCodeToList(clist, RegisterNameToBinary(value), 'A');
		break;
	}

	}
}

void printCodeListRow(CodeListRow* row)
{
	unsigned int twelveBits = row->word & 0xFFF;
	printf("Address: %d  word: %x ARE: %c\n", row->address, twelveBits, row->ARE);
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