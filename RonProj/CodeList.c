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
CodeList* initCodeList() 
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

}
void addCodeToList(CodeList* clist, int address, unsigned int word, char ARE)
{
	if (!clist)
	{
		return;
	}
	addToList((void*)createCodeRow(address, word, ARE), clist->list);
}

void freeCodeList(CodeList* clist)
{
	if (!clist)
		return;

	freeList(clist->list, freeCodeListRow);
	free(clist);
}