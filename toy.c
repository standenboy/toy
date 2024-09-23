#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum dir {
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

enum token {
	NOP = ' ',
	PUSH = 'p',
	ENDPUSH = 'P',
	ADD = '+',
	SUB = '-',
	WRT = 'w',
	READ = 'r',
	END = 'e',
	FLOWUP = '^',
	FLOWDOWN = 'v',
	FLOWLEFT = '<',
	FLOWRIGHT = '>',
	ISZERO = 'z',
	INC = 'i',
	DEC = 'd',
	CPY = 'c',
};

int *sp;

void push(int data){
	*sp = data; 
	sp += sizeof(int); 
}

int pop(){
	sp -= sizeof(int);
	int data = *sp;
	*sp = 0;
	return data;
}

int main(int argc, char **argv){
	if (argc < 2) return 1;
	
	FILE *f = fopen(argv[1], "r");
	fseek(f, 0, SEEK_END);
	size_t len = ftell(f);
	rewind(f);
		
	char *buf = malloc(len) + 1;

	fread(buf, 1, len, f);

	char **file = malloc(sizeof(char**));
	int i = 0;
	char *tmp = malloc(256);
	tmp = strtok(buf, "\n");
	file[i] = malloc(strlen(tmp)+1);
	strcpy(file[i],  tmp);
	tmp = strtok(NULL, "\n");
	while (tmp != NULL){
		i++;
		file[i] = malloc(strlen(tmp)+1);
		strcpy(file[i],  tmp);
		tmp = strtok(NULL, "\n");
	}
	
	enum dir readdir = RIGHT;	
	int row = 0, col = 0;
	enum token t = NOP;

	int *stack = calloc(0, sizeof(int) * 1024);
	sp = stack;
	
	while (t != END){
		t = file[row][col];
		int lhs = 0;
		int rhs = 0;
		char c = 0;
		char *num = calloc(0, 12);
		int i = 0;
		switch (t){
			case NOP: break;
			case FLOWUP: readdir = UP; break;
			case FLOWDOWN: readdir = DOWN; break;
			case FLOWLEFT: readdir = LEFT; break;
			case FLOWRIGHT: readdir = RIGHT; break;
			case PUSH:
				switch (readdir) {
					case RIGHT: col++; break;
					case LEFT: col--; break;
					case UP: row--; break;
					case DOWN: row++; break;
				}
				while ((c = file[row][col]) != ENDPUSH){
					num[i] = c;
					i++;
					switch (readdir) {
						case RIGHT: col++; break;
						case LEFT: col--; break;
						case UP: row--; break;
						case DOWN: row++; break;
					}
				}
				num[i] = '\0';
				c = strtol(num, NULL, 0); 
				push(c);
				break;	
			case WRT: 
				c = pop();
				putchar(c); 
				break;
			case READ: 
				push(getchar());
				while (getchar() != '\n'); 
				break;
			case ADD: 
				lhs = pop();
				rhs = pop();
				push(lhs+rhs);
				break;
			case SUB: 
				lhs = pop();
				rhs = pop();
				push(lhs-rhs);
				break;
			case ISZERO:
				c = pop();
				if (c == 0) readdir = UP;
				else readdir = DOWN;
				break;
			case INC: 
				c = pop();
				push(c+1);
				break;
			case DEC: 
				c = pop();
				push(c-1);
				break;
			case CPY:
				c = pop();
				push(c);
				push(c);
				break;
			case END:
				continue;	
				break;
		}
		switch (readdir) {
			case RIGHT: col++; break;
			case LEFT: col--; break;
			case UP: row--; break;
			case DOWN: row++; break;
		}
	}	

	return 0;
}
