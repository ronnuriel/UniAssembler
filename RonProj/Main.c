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
	//CodeList* l = initCodeList(100);
	//char* s1 = "0";
	//char* s2 = "1";
	//char* s3 = "-1";
	//char* s[3] = { s1,s2, s3 };
	//addDataToCodeList(l, s,3);

	//printCodeList(l);
	//SymbolList *l = initSymbolList();
	//addSymbolToList(l, "abc", 123, 'a');
	//printSymbolList(l);
	//printf("%d", generateBinaryWord(ADD, REGISTER_DIRECT, DIRECT));
	int i;
	/*for (i = 1; i < argc; i++)
	{
		printf("working on %s\n", argv[i]);
		compileFile(argv[i]);
	}*/
	compileFile("test");
	printf("\ndone.\n");
}