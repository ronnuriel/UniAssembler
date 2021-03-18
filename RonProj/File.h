#ifndef  FILE_H
#define FILE_H
#include <stdio.h>
#include <stdlib.h>

#define FILE_ADDR_LEN 4
#define FILE_VALUE_LEN 3


int openInputFile(char* path);
void closeInputFile();
int readNextLine(char* str, int maxLength);
int rewindInputFile();

int openOutputFile(char* path);
void closeOutputFile();
int writeOutput(char* str);

int createFileNames(char* filename, char** as, char** ob, char** ent, char** ext);

#endif