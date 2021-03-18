#ifndef  COMPILER_H
#define COMPILER_H
#include "SymbolList.h"
#include "CodeList.h"

int compileFile(char* inputFilePath);
int compileInstruction(char* line, SymbolList* symbolList, CodeList* dataList);
int compileOperation(char* line, SymbolList* symbolList, CodeList* operationList);
void updateEntries(SymbolList* symbolList);

int generateObjectFile(CodeList* operationList, CodeList* dataList, char* path);
int generateAttributeFile(SymbolList* symbolList, char* path, SymbolAttributesEnum attribute);
int generateExternalFile(CodeList* operationList, char* path);
#endif