 typedef struct {
	int top;
	int start;
	int end;
} Stack;

void push(Stack* stack, int* buffer, int data);
int pop (Stack* stack, int* buffer);
int peek(Stack* stack, int* buffer);
void sswitch(Stack* stack, int* buffer);

void print_help(char* pname);
char* argparse(int argc, char** argv);
