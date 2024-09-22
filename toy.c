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
	int *sp = stack;
	
	while (t != END){
		t = file[row][col];
		int lhs = 0;
		int rhs = 0;
		char c = 0;
		switch (t){
			case NOP: break;
			case FLOWUP: readdir = UP; break;
			case FLOWDOWN: readdir = DOWN; break;
			case FLOWLEFT: readdir = LEFT; break;
			case FLOWRIGHT: readdir = RIGHT; break;
			case WRT: 
				sp -= sizeof(int);
				c = *sp;
				*sp = 0;
				putchar(c); 
				break;
			case READ: 
				*sp = getchar(); 
				sp += sizeof(int); 
				while (getchar() != '\n'); 
				break;
			case ADD: 
				sp -= sizeof(int);
				lhs = *sp;
				*sp = 0;
				sp -= sizeof(int);
				rhs = *sp;
				*sp = 0;
				*sp = lhs + rhs;
				sp += sizeof(int);
				break;
			case SUB: 
				sp -= sizeof(int);
				lhs = *sp;
				*sp = 0;
				sp -= sizeof(int);
				rhs = *sp;
				*sp = 0;
				*sp = lhs - rhs;
				sp += sizeof(int);
				break;
			case ISZERO:
				sp -= sizeof(int);
				c = *sp;
				if (c == 0) readdir = UP;
				else readdir = DOWN;
				*sp = 0;
				break;
			case INC: 
				sp -= sizeof(int);
				c = *sp;
				*sp = 0;
				*sp = c + 1;
				sp += sizeof(int);
				break;
			case DEC: 
				sp -= sizeof(int);
				c = *sp;
				*sp = 0;
				*sp = c - 1;
				sp += sizeof(int);
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
