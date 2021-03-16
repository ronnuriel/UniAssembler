#include "CodeList.h"
#include <stdlib.h>


CodeListRow* createCodeListRow(int address, unsigned int word, char ARE)
{

	CodeListRow* ret = (CodeListRow*)malloc(sizeof(CodeListRow));
	if (!ret)
		return NULL;
	ret->address = address;
	ret->word = word;
	ret->ARE = ARE;

	return ret;

}
void freeCodeListRow(CodeListRow* row)
{
	free(row);
}
CodeList* initCodeList(int startAddr) 
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

void addCodeToList(CodeList* clist, unsigned int word, char ARE)
{
	if (!clist)
	{
		return;
	}
	addToList((void*)createCodeListRow(clist->currAddr, word, ARE), clist->list);
	(clist->currAddr)++;
}

void freeCodeList(CodeList* clist)
{
	if (!clist)
		return;

	freeList(clist->list, freeCodeListRow);
	free(clist);
}
int getCodeListCurrentAddr(CodeList* clist)
{
	return clist->currAddr;
}

void addStringToCodeList(CodeList* clist, char* str)
{
	for (int i = 0; i < strlen(str)+1; i++) // +1 to include \0
	{
		addCodeToList(clist, str[i], "A");
	}
}
void addDataToCodeList(CodeList* clist, char** params, int numParams)
{
	for (int i = 0; i < numParams; i++) 
	{
		char* paramStr = params[i];

		addCodeToList(clist, atoi(paramStr), "A");
	}
}