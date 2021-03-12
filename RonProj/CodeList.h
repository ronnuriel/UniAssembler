#ifndef CODE_LIST_H
#define CODE_LIST_H

#include "List.h"
typedef struct {
	int address;
	unsigned int word; // word is 12 bits. int will have enough space
	char ARE;
} CodeListRow;

typedef struct {
	List* list;
} CodeList;

CodeListRow* createCodeListRow(int address, unsigned int word, char ARE);
void freeCodeListRow(CodeListRow* row);
CodeList* initCodeList();
void addCodeToList(CodeList* clist, int address, unsigned int word, char ARE);
void freeCodeList(CodeList* clist);

#endif