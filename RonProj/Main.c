#include <stdio.h>
#include "File.h"
#include "HelperFunc.h"
#include "Parser.h"
#include "Operator.h"
#include "SymbolList.h"
#include "CodeList.h"
#include "Compiler.h"
int main(int argc, char** argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		printf("working on %s\n", argv[i]);
		compileFile(argv[i]);
	}
	
	printf("\ndone.\n");
}