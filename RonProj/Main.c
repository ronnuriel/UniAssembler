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
		if (compileFile(argv[i]))
		{
			printf("%s compiled successfully\n", argv[i]);
		}
		else
		{
			printf("Error was found during compilation of %s.\n", argv[i]);
		}
	}

	char* test = "test";
	printf("working on %s\n", test);
	if (compileFile(test))
	{
		printf("%s compiled successfully\n", test);
	}
	else
	{
		printf("Error was found during compilation of %s.\n", test);
	}

	printf("\ndone.\n");
	return 0;
}