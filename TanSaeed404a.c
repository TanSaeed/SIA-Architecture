#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//cd
//gcc -o #nameit file.c 
// ./a3.c #inputfile.txt #outputfile.txt
// xxd output.sia

// Tanzeel Saeed CS 404 Assembler Assignment 
//


char *ltrim(char *s) {
	while (*s == ' ' || *s == '\t') s++;
	return s;
}

char getRegister(char *text) {
	if (*text == 'r' || *text=='R') text++;
	return atoi(text);
}

int assembleLine(char *text, unsigned char* bytes) { 
	text = ltrim(text);
	char *keyWord = strtok(text," ");
	if (strcmp("add",keyWord) == 0) 
	{
		bytes[0] = 0x10; // op code for add
		bytes[0] |= getRegister(strtok(NULL," ")); //or = with the value of the register
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," ")); //gets the second register shifted to the left, then gets
																					// Last register  
		return 2; //2 byte return
	}
	else if (strcmp("addimmediate",keyWord) == 0) 
	{ 
		bytes[0] = 0x90; // op code for addimmediate
		bytes[0] |= getRegister(strtok(NULL," ")); //or = with the value of the register
		bytes[1] = atoi(strtok(NULL," ")); // gets value of immediate number and stores as 32 bit int but we only get the first 8 bits.
		return 2;
	}
	else if (strcmp("and",keyWord) == 0) 
	{ 
		bytes[0] = 0x20; // op code for and
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," ")); // same as add 3r
		return 2;
	}
	else if (strcmp("branchifequal",keyWord) == 0)
	{ 
		bytes[0] = 0xA0; // op code
		bytes[0] |= getRegister(strtok(NULL," ")); //or = with the value of the register
		bytes[1] = getRegister(strtok(NULL," ")) << 4; // gets next register shifted to the left spot of the byte.
		int x = atoi(strtok(NULL," "));
		bytes[1] |= x >> 16;  // or= with int x shifted to the right 16 so we get the right values to store in byte 1
		bytes[2]= x >> 8;    
		bytes[3]= x;
		return 4; //4 byte return
	}
	else if (strcmp("branchifless",keyWord) == 0)
	{ 
		bytes[0] = 0xB0; // same as branchifequal but diffrent opcode
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4;
		int x = atoi(strtok(NULL," "));
		bytes[1] |= x >> 16;  
		bytes[2]= x >> 8;    
		bytes[3]= x;
		return 4;
	}
	else if (strcmp("divide",keyWord) == 0) 
	{ 
		bytes[0] = 0x30;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," ")); // same as any other 3r format, just diffrent opcode
		return 2;
	}
	else if (strncmp("halt",keyWord,4) == 0) //used strncmp because c has a return if nothing is on the end of string
	{ 
		bytes[0] = 0x00;
		bytes[1] = 0x00; // halt brings back only 0s for both bytes.
		
		return 2;
	}
	else if (strcmp("interrupt",keyWord) == 0) 
	{ 
		bytes[0] = 0x80;
		int x = atoi(strtok(NULL," "));
		bytes[0] |= x >> 8; // shifts left 8 so we get the next 8 bits
		bytes[1] = x; // 8 bits at the start
		return 2;
	}
	else if (strcmp("iterateover",keyWord) == 0) 
	{ 
		bytes[0] = 0xD0;
		bytes[0] |= getRegister(strtok(NULL," "));
		int x = atoi(strtok(NULL," "));
		bytes[1] = x;
		x = atoi(strtok(NULL," "));
		bytes[2] = x >> 8;
		bytes[3] = x;
		return 4;
	}
	else if (strcmp("jump",keyWord) == 0) 
	{ 
		bytes[0] = 0xC0;
		int x = atoi(strtok(NULL," "));
		bytes[0] |= x >> 24; //gets the proper bits after the first 16
		bytes[1] = x >> 16;
		bytes[2] = x >> 8;
		bytes[3] = x;
		return 4;
	}
	else if (strcmp("leftshift",keyWord) == 0) 
	{ 
		bytes[0] = 0x70;
		bytes[0] |= getRegister(strtok(NULL," ")); // or= with the opcode and register
		bytes[1] = atoi(strtok(NULL," ")); // value of shift by goes here
		
		return 2;
	}
	else if (strcmp("load",keyWord) == 0) 
	{ 
		bytes[0] = 0xE0;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4; // similar to 3r format, instead of another register, we have a value.
		bytes[1] |= atoi(strtok(NULL," "));
		
		return 2;
	}
	else if (strcmp("multiply",keyWord) == 0) 
	{ 
		bytes[0] = 0x40;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}
	else if (strcmp("or",keyWord) == 0) 
	{ 
		bytes[0] = 0x60;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}
	else if (strcmp("rightshift",keyWord) == 0) 
	{ 
		bytes[0] = 0x70;
		bytes[0] |= getRegister(strtok(NULL," "));
		int x = atoi(strtok(NULL," "));
		bytes[1] = x << 5; // right shift has a 0 or a 1 value in the 5th spot in the second byte so this shift set covers that as the first most left bit
	bytes[1] |= x; // or equals with the rest of the shift value
		
		return 2;
	}
	else if (strcmp("store",keyWord) == 0) 
	{ 
		bytes[0] = 0xF0;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4; //like load but diffrent opcode
		bytes[1] |= atoi(strtok(NULL," "));
		
		return 2;
	}
	else if (strcmp("subtract",keyWord) == 0) 
	{ 
		bytes[0] = 0x50;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}
	
}

int main(int argc, char **argv) {
	FILE *src = fopen(argv[1],"r");
	FILE *dst = fopen(argv[2],"wb");
	while (!feof(src)) {
		unsigned char bytes[4];
		char line[1000];
		printf ("about to read\n");
		if (NULL != fgets(line, 1000, src)) {
			printf ("read: %s\n",line);
			int byteCount = assembleLine(line,bytes);
			fwrite(bytes,byteCount,1,dst);
		}
	}
	fclose(src);
	fclose(dst);
	return 0;
}