#ifndef  FILE_H
#define FILE_H
#include <stdio.h>
#include <stdlib.h>



int openInputFile(char* path);
void closeInputFile();
int readNextLine(char* str);


#endif