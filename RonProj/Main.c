#include <stdio.h>
#include "File.h"
#include "HelperFunc.h"
#include "Parser.h"
#include "Operator.h"
int main(int argc, char** argv)
{

	//stripOperandData
	char c[] = "LABEL: bne";
	Operation o = parseOperation(c);
}