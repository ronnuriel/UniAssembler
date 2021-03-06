#ifndef  FILE_H
#define FILE_H
#include <stdio.h>
#include <stdlib.h>



int openInputFile(char* path);
void closeInputFile();
int readNextLine(char* str, int maxLength);
int rewindInputFile();


int createFileNames(char* filename, char** as, char** ob, char** ent, char** ext);

#endif