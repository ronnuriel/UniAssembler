

#include "Compiler.h"
#include "File.h"

int compileFile(char* inputFilePath)
{

	if(!openInputFile(inputFilePath))
	{
		return -1;
	}	


	

	closeInputFile();

}