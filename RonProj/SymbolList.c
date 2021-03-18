#define _CRT_SECURE_NO_WARNINGS
#include "SymbolList.h"
#include <stdlib.h>
#include <string.h>
#include "Symbol.h"
#include "File.h"
#include "stdio.h"

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
int getSymbolValueByName(SymbolList* slist, char* name, int* returnValue)
{
	SymbolListRow* row = getSymbolRowByName(slist, name);
	if (!row)
		return 0;
	
	*returnValue = row->value;
	return 1;
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
int isRowOfType(SymbolListRow* row, SymbolAttributesEnum attribute)
{
	return row->attributes & attribute;
}
void updateSymbolListValuesOfData(SymbolList* slist, int incVal)
{
	if (!slist)
		return;

	Node* t = slist->list->head;

	while (t)
	{
		SymbolListRow* row = t->data;
		if (isRowOfType(row, DATA))
		{
			row->value += incVal;
		}
		t = getNodeNext(t);
	}
}

int addAttributeToSymbolInSymbolList(SymbolList* slist, char* symbol, SymbolAttributesEnum attribute)
{
	SymbolListRow* row = getSymbolRowByName(slist, symbol);
	if (!row)
		return 0;

	row->attributes |= attribute;
	return 1;
}


void printSymbolListRow(SymbolListRow* row)
{
	printf("label: %s  value: %d, attributes:", row->name, row->value);
	printSymbolAttributes(row->attributes);
	printf("\n");
}
void printSymbolList(SymbolList* slist)
{
	if (!slist)
	{
		printf("No symbol list\n");
		return;
	}
	int len = slist->list->length;
	printf("symbol list: length %d.\n", len);
	printf("================================\n");
	Node* t = slist->list->head;

	while (t)
	{
		SymbolListRow* row = t->data;
		printSymbolListRow(row);
		t = getNodeNext(t);
	}

	printf("\n=== End of symbol list ====\n\n");

}



void printSymbolAttributes(unsigned char attributes)
{
	if (attributes & CODE)
		printf(" CODE, ");
	if (attributes & DATA)
		printf(" DATA, ");
	if (attributes & EXTERNAL)
		printf(" EXTERNAL, ");
	if (attributes & ENTRY)
		printf(" ENTRY, ");

}
int doesSymbolListIncludeAttribute(SymbolList* slist, SymbolAttributesEnum attribute)
{
	if (!slist)
	{
		return 0;
	}
	Node* t = slist->list->head;
	while (t)
	{
		SymbolListRow* row = t->data;
		if (row->attributes & attribute)
		{
			/* attribute present */
			return 1;
		}
		t = getNodeNext(t);
	}
	return 0;
}
void printSymbolListToFuncByAttribute(SymbolList* slist, SymbolAttributesEnum attribute, int func(char* str))
{
	if (!slist)
	{
		return;
	}

	Node* t = slist->list->head;
	while (t)
	{
		SymbolListRow* row = t->data;
		if (row->attributes & attribute)
		{
			/* attribute present */
			char str[MAX_SYMBOL_LEN + 1 + FILE_ADDR_LEN + 1 + 1];
			sprintf(str, "%s %.4d\n", row->name, row->value);
			func(str);
		}
		t = getNodeNext(t);
	}

}