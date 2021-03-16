#ifndef  COMPILER_H
#define COMPILER_H
#include "SymbolList.h"
#include "CodeList.h"

int compileFile(char* inputFilePath);
int compileInstruction(char* line, SymbolList* symbolList, CodeList* dataList);
int compileOperation(char* line, SymbolList* symbolList, CodeList* operationList);
#endif