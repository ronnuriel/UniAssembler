#include "CodeList.h"
#include <stdlib.h>


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