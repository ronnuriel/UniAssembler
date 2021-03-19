# Basic compilation macros
CC = gcc # GCC Compiler
CFLAGS = -ansi -Wall -pedantic # Flags
GLOBAL_DEPS =# Dependencies for everything
EXE_DEPS = CodeList.o Compiler.o File.o HelperFunc.o Instruction.o List.o Main.o Node.o Operator.o Parser.o Register.o Symbol.o SymbolList.o # Deps for exe

## Executable
Main: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

## Main:
Main.o: Main.c $(GLOBAL_DEPS)
	$(CC) -c Main.c $(CFLAGS) -o $@

## Code helper functions:
CodeList.o: CodeList.c CodeList.h $(GLOBAL_DEPS)
	$(CC) -c CodeList.c $(CFLAGS) -o $@

## First Pass:
Compiler.o: Compiler.c Compiler.h $(GLOBAL_DEPS)
	$(CC) -c Compiler.c $(CFLAGS) -o $@

## Second Pass:
File.o: File.c File.h $(GLOBAL_DEPS)
	$(CC) -c File.c $(CFLAGS) -o $@

## Instructions helper functions:
HelperFunc.o: HelperFunc.c HelperFunc.h $(GLOBAL_DEPS)
	$(CC) -c HelperFunc.c $(CFLAGS) -o $@

## Table:
Instruction.o: Instruction.c Instruction.h $(GLOBAL_DEPS)
	$(CC) -c Instruction.c $(CFLAGS) -o $@

## Useful functions:
List.o: List.c List.h $(GLOBAL_DEPS)
	$(CC) -c List.c $(CFLAGS) -o $@

## Output Files:
Node.o: Node.c Node.h $(GLOBAL_DEPS)
	$(CC) -c Node.c $(CFLAGS) -o $@
## Output Files:
Operator.o: Operator.c Operator.h $(GLOBAL_DEPS)
	$(CC) -c Operator.c $(CFLAGS) -o $@

## Output Files:
Parser.o: Parser.c Parser.h $(GLOBAL_DEPS)
	$(CC) -c Parser.c $(CFLAGS) -o $@
## Output Files:
Register.o: Register.c Register.h $(GLOBAL_DEPS)
	$(CC) -c Register.c $(CFLAGS) -o $@
## Output Files:
Symbol.o: Symbol.c Symbol.h $(GLOBAL_DEPS)
	$(CC) -c Symbol.c $(CFLAGS) -o $@
## Output Files:
SymbolList.o: SymbolList.c SymbolList.h $(GLOBAL_DEPS)
	$(CC) -c SymbolList.c $(CFLAGS) -o $@

# Clean Target (remove leftovers)
clean:
	rm -rf *.o