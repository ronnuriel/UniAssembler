#ifndef CODE_LIST_H
#define CODE_LIST_H

#include "List.h"
#include "Parser.h"
#include "Operator.h"
typedef struct {
	int address;
	unsigned int word; /*word is 12 bits. int will have enough space*/
	char ARE;
} CodeListRow;

typedef struct {
	List* list;
	int currAddr;
} CodeList;

CodeListRow* createCodeListRow(int address, unsigned int word, char ARE);
void freeCodeListRow(CodeListRow* row);
CodeList* initCodeList(int startAddr);

int getCodeListCurrentAddr(CodeList* clist);
void addCodeToList(CodeList* clist, unsigned int word, char ARE);
void freeCodeList(CodeList* clist);

void addStringToCodeList(CodeList* clist, char* str);
void addDataToCodeList(CodeList* clist, char** params, int numParams);

void addOperationToCodeList(CodeList* clist, Operation* op);
void addOperandToCodeList(CodeList* clist, AddrMethodEnum type, char* value);

void printCodeListRow(CodeListRow* row);
void printCodeList(CodeList* clist);

#endif