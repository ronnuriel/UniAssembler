#ifndef PARSER_FUNC
#define PARSER_FUNC
typedef enum 
{
	INVALID_LINE = -1, 
	EMPTY_LINE,
	COMMENT_LINE,
	INSTRUCTION_LINE,
	OPERATION_LINE
} LineTypeEnum;

LineTypeEnum detectLineType(char* line);

int parseIntruction(char* line);
int parseOperation(char* line);


#endif