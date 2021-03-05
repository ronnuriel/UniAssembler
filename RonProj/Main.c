#include <stdio.h>
#include "File.h"
#include "HelperFunc.h"
int main(int argc, char** argv)
{
	char* as, * ob, * ent, * ext;
	createFileNames("1234", &as, &ob, &ent, &ext);
	printf("%s\n", as);
	printf("%s\n", ob);
	printf("%s\n", ent);
	printf("%s\n", ext);

	//for (int i = 1; i < argc; i++)
	//{
	//	printf("Working on file: %s\n", argv[i]);
	//	compileFile(argv[i]);


	//}
	return 0;
}