#ifndef CODE_LIST_H
#define CODE_LIST_H

#include "List.h"
#include "Parser.h"
#include "Operator.h"
#include "SymbolList.h"

typedef struct {
	int address;
	unsigned int word; // word is 12 bits. int will have enough space
	char ARE;
	char data[MAX_SYMBOL_LEN]; /* will hold label name in case of operand with label*/
} CodeListRow;

typedef struct {
	List* list;
	int currAddr;
} CodeList;


typedef enum {
	ARE_A = 'A',
	ARE_R = 'R',
	ARE_E = 'E',
	ARE_RELATIVE = 'R',
	ARE_DIRECT = 'D'
} AREEnum;

CodeListRow* createCodeListRow(int address, unsigned int word, char ARE, char* data);
void freeCodeListRow(CodeListRow* row);
CodeList* initCodeList(int startAddr);

int getCodeListCurrentAddr(CodeList* clist);
int getCodeListLength(CodeList* clist);
void addCodeToList(CodeList* clist, unsigned int word, char ARE, char* data);
void freeCodeList(CodeList* clist);

void addStringToCodeList(CodeList* clist, char* str);
void addDataToCodeList(CodeList* clist, char** params, int numParams);

void addOperationToCodeList(CodeList* clist, Operation* op);
void addOperandToCodeList(CodeList* clist, AddrMethodEnum type, char* value);

int updateRelativeAndDirectLabelsInCodeList(CodeList* clist, SymbolList* symbolList);

void printCodeListRow(CodeListRow* row);
void printCodeList(CodeList* clist);

int printCodeListToFunc(CodeList* clist, int offset, int func(char* str));
int printCodeListExternalsToFunc(CodeList* clist, int func(char* str));
int doesCodeListIncludeExternals(CodeList* clist);
#endif