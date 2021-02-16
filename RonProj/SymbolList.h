#ifndef SYMBOL_LIST_H
#define SYMBOL_LIST_H


typedef enum
{
	CODE = 0x1,
	DATA = (0x1 << 1),
	EXTERNAL = (0x1 << 2),
	ENTRY = (0x1 << 3)

} SymbolAttributesEnum;



typedef struct {
	char* name;
	int value;
	int attributes;
} SymbolListColumn;

List* initList();
#endif
