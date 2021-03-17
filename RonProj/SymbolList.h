#ifndef SYMBOL_LIST_H
#define SYMBOL_LIST_H

#include "List.h"
/* checked */
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
	unsigned char attributes;
} SymbolListRow;

typedef struct {
	List* list;
} SymbolList;

/* Symbol List Row*/
SymbolListRow* createSymbolListRow(char* name, int value, unsigned char attributes);
void freeSymbolListRow(SymbolListRow* row);


// returns 1 if match
int isRowNameMatch(char* name, SymbolListRow* row);


/* Symbol List */
SymbolList* initSymbolList();
void addSymbolToList(SymbolList* slist, char* name, int value, unsigned char attributes);
void freeSymbolList(SymbolList* slist);
SymbolListRow* getSymbolRowByName(SymbolList* slist, char* name);

void printSymbolAttributes(unsigned char attributes);
void printSymbolListRow(SymbolListRow* row);
void printSymbolList(SymbolList* slist);
#endif
