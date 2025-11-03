#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bff.h"

#define TRUE 1
#define FALSE 0

#define MEMSIZE 30000

#define REG MEMORY[0]
#define RG2 MEMORY[1]
#define RG3 MEMORY[2]
#define IND MEMORY[3]
#define EXE MEMORY[4]
#define SIN MEMORY[5]
#define SEX MEMORY[6]
#define RET MEMORY[7]
#define TOP MEMORY[8]
#define STK MEMORY[9]

void push(Stack* stack, int* buffer, int data){
	stack->top++;
	if (stack->top > stack->end) stack->top = stack->start;
	buffer[stack->top] = data;
}

int pop (Stack* stack, int* buffer){
	int data = buffer[stack->top];
	stack->top--;
	if (stack->top < stack->start) stack->top = stack->end;
	return data;
};

int peek(Stack* stack, int* buffer) {
	return buffer[stack->top];
};

void sswitch(Stack* stack, int* buffer) {
	int data1 = pop(stack, buffer);
	int data2 = pop(stack, buffer);

	push(stack, buffer, data1);
	push(stack, buffer, data2);
};

void print_help(char* pname){
	printf("brainfuckfuck: superset of brainfuck, by fryedbicuit\n\
		USAGE: %s filename\n\t\tpreloads code into memory tape\n", pname);
}

char* argparse(int argc, char** argv) {
	// deal with arguments
	if (argc == 1) {
		print_help(argv[0]);
		exit(0);
	} else if (strcmp("-h", argv[1]) == 0){
		print_help(argv[0]);
		exit(0);
	} else if (strcmp("--help", argv[1]) == 0 ) {
		print_help(argv[0]);
		exit(0);
	} else {
		return argv[1];
	};
};

int main(int argc, char** argv) {
	// Get the filename and open the *.bf file containing code
	char* codefile = argparse(argc,argv);
	int fd = open(codefile, O_RDONLY);

	// alocate the MEMORY block
	int* MEMORY = calloc(MEMSIZE, sizeof(int));

	// Define where code should be put and run from
	int codestart = 2010;

	// Set exec starting registers
	EXE = SEX = codestart;

	// test codefile
	if (fd == -1) { printf("ERROR: file \"%s\" does not exist",argv[1]); exit(1); };

	// Load codefile into memory starting at cell codestart
	int count = 0;
	char buffer = 0;
	while (read(fd, &buffer, 1)) {
		MEMORY[codestart] = (int)buffer;
		codestart++;
		count++;
	}

	// Set index registers
	IND = SIN = SEX + count + 2;

	// Define the stacks
	Stack* stack = (Stack*) malloc(sizeof(Stack));
		stack->start = 10;
		stack->end = 1009;
		stack->top = 10;

	Stack* recstk = (Stack*) malloc(sizeof(Stack));
		stack->start = 1010;
		stack->end = 2009;
		stack->top = 1010;
	
	while (TRUE) {
		int cell = MEMORY[EXE];
		if (cell <= 255) {
			switch (cell){
				case '+': // Increase cell
					MEMORY[IND]++;
					break;
				case '-': // decrease cell
					MEMORY[IND]--;
					break;
				case '<': // move pointer left
					IND--;
					break;
				case '>': // move pointer right
					IND++;
					break;
				case ',': // getchar
					MEMORY[IND] = getchar();
					break;
				case '.': // putchar
					putchar(MEMORY[IND]);
					break;
				case '?': // putchar
					printf("%d",MEMORY[IND]);
					break;
				case '[': // start loop
					push(recstk, MEMORY, EXE);
					break;
				case ']': // if !=0 the goto [
					if (MEMORY[IND] == 0){
						pop(recstk,MEMORY);
						break;
					} else {
						EXE = peek(recstk,MEMORY);
						break;
					};
				case '=': // set register
					REG = MEMORY[IND];
					break;
				case '_': // get register
					MEMORY[IND] = REG;
					break;
				case '$': // push register into stack
					push(stack,MEMORY,REG);
					break;
				case '#': // pop into reg
					REG = pop(stack,MEMORY);
					break;
				case '&': // peek into reg
					REG = peek(stack,MEMORY);
					break;
				case '%': // switch top with top-1
					sswitch(stack,MEMORY);
					break;
				case '\0': // exit returning 0
					exit(0);
					break;
				default:
					break;
			};

		}

		EXE++;
	};


}
