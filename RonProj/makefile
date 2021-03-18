OBJS	= CodeList.o Compiler.o File.o HelperFunc.o Instruction.o List.o Main.o Node.o Operator.o Parser.o Register.o Symbol.o SymbolList.o
SOURCE	= CodeList.c Compiler.c File.c HelperFunc.c Instruction.c List.c Main.c Node.c Operator.c Parser.c Register.c Symbol.c SymbolList.c
HEADER	= CodeList.h Compiler.h File.h HelperFunc.h Instruction.h List.h Main.h Node.h Operator.h Parser.h Register.h Symbol.h SymbolList.h
OUT	= aaa
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

CodeList.o: CodeList.c
	$(CC) $(FLAGS) CodeList.c 

Compiler.o: Compiler.c
	$(CC) $(FLAGS) Compiler.c 

File.o: File.c
	$(CC) $(FLAGS) File.c 

HelperFunc.o: HelperFunc.c
	$(CC) $(FLAGS) HelperFunc.c 

Instruction.o: Instruction.c
	$(CC) $(FLAGS) Instruction.c 

List.o: List.c
	$(CC) $(FLAGS) List.c 

Main.o: Main.c
	$(CC) $(FLAGS) Main.c 

Node.o: Node.c
	$(CC) $(FLAGS) Node.c 

Operator.o: Operator.c
	$(CC) $(FLAGS) Operator.c 

Parser.o: Parser.c
	$(CC) $(FLAGS) Parser.c 

Register.o: Register.c
	$(CC) $(FLAGS) Register.c 

Symbol.o: Symbol.c
	$(CC) $(FLAGS) Symbol.c 

SymbolList.o: SymbolList.c
	$(CC) $(FLAGS) SymbolList.c 


clean:
	rm -f $(OBJS) $(OUT)
