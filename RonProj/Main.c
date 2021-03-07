#include <stdio.h>
#include "File.h"
#include "HelperFunc.h"
#include "Parser.h"
#include "Operator.h"
int main(int argc, char** argv)
{
	
	printf("%d\n", detectOperandType("r9"));
	printf("%d\n", detectOperandType("#1.234"));
	printf("%d\n", detectOperandType("%1ABEL"));
	printf("%d\n", detectOperandType("1ABEL"));
}