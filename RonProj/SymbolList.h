#ifndef SYMBOL_LIST_H
#define SYMBOL_LIST_H

#include "List.h"

typedef enum
{
	CODE = 0x1,
	DATA = (0x1 << 1),
	EXTERNAL = (0x1 << 2),
	ENTRY = (0x1 << 3)

} SymbolAttributesEnum;



typedef struct {
	char* name;
	int value;
	SymbolAttributesEnum attributes;
} SymbolListRow;

typedef struct {
	List* list;
} SymbolList;

SymbolListRow* createSymbolListRow(char* name, int value, SymbolAttributesEnum attributes);
void freeSymboleListRow(SymbolListRow* row);

// returns 1 if match
int isRowNameMatch(char* name, SymbolListRow* row);

SymbolList* initSymbolList();
void freeSymbolList(SymbolList* slist);
#endif
