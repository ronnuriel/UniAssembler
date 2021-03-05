#define _CRT_SECURE_NO_WARNINGS
#include "SymbolList.h"
#include <stdlib.h>
#include <string.h>
SymbolListRow* createSymbolListRow(char* name, int value, unsigned char attributes)
{
	if (!name)
		return NULL;

	SymbolListRow* ret = (SymbolListRow*)malloc(sizeof(SymbolListRow));
	if (!ret)
		return NULL;

	ret->name = (char*)malloc(sizeof(strlen(name)+1));
	if (!ret->name)
	{
		free(ret);
		return NULL;
	}

	strcpy(ret->name, name);
	ret->value = value;
	ret->attributes = attributes;

	return ret;
}

void freeSymbolListRow(SymbolListRow* row)
{
	if (!row)
		return;

	free(row->name);
	free(row);
}

// returns 1 if match
int isRowNameMatch(char* name, SymbolListRow* row)
{
	if (!name || !row)
		return 0;

	return !strcmp(name, row->name);
}

SymbolList* initSymbolList()
{
	SymbolList* ret = (SymbolList*)malloc(sizeof(SymbolList));
	if (!ret)
		return NULL;

	ret->list = initList();
	if (!ret->list)
	{
		free(ret);
		return NULL;
	}
}
SymbolListRow* getSymbolRowByName(SymbolList* slist, char* name)
{
	if (!slist)
		return NULL;

	Node* t = slist->list->head;

	while (t)
	{
		SymbolListRow* row = t->data;
		if (isRowNameMatch(name, row))
		{
			return row;
		}
		t = getNodeNext(t);
		
	}

	return NULL;
}

void addSymbolToList(SymbolList* slist, char* name, int value, unsigned char attributes)
{
	if (!slist || !name)
		return;

	addToList((void*)createSymbolListRow(name, value, attributes), slist->list);
}

void freeSymbolList(SymbolList* slist)
{
	if (!slist)
		return;

	freeList(slist->list, freeSymbolListRow);
	free(slist);
}