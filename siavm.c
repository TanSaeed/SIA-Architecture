#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Tanzeel Saeed siavm 
//Phipps 404
//11/19/2018


unsigned char mem[10000]; // Memory Allocation

void initiate(char* fname);
void fetch();
void dispatch();
void execute();
void store();
void run();
void printMemory(unsigned char *c);


int regi[16]; 
int op1, op2;
int result;

int pc = 0;
char cinstruct[4]; // instruction buffer
int offset; // address offset used to grab off sets for 32 bit operations that require it
int temp; // used in dispatch to format and help move bits around

unsigned char ophold; // to hold opcode to make it easier fo switch statements
int running = 1; // if the program is running 

void initiate(char* fname){
	FILE* f = fopen(fname, "r");
	
	fread(mem, 1, 10000, f);
	fclose(f);
}



void fetch(){	
	cinstruct[0] = mem[pc];
	cinstruct[1] = mem[pc+1];
	cinstruct[2] = mem[pc+2];
	cinstruct[3] = mem[pc+3];
}


void dispatch(){
	ophold = (cinstruct[0] >> 4) & 0x0F; // shifts over 0010 0000 > ffff 0010. Then anded with f so you get 
	                                     // ffff 0010
										 // 0000 1111 = 0000 0010
	
	switch(ophold){
		//3R
		// halt code
		case 0:
		//
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		op1 = regi[(cinstruct[0] & 0x0F)];
		op2 = regi[(cinstruct[1] >> 4) & 0x0F];
		break;
		
		//sft
		case 7:
		op1 = regi[(cinstruct[0] & 0x0F)]; 
		op2 = (cinstruct[1] & 0x1F); // to get all 5 bits
		break;
		
		// int 
		case 8:
		op1 = cinstruct[0];
		op1 = (op1 << 8);
		op1 = regi[(op1 | cinstruct[1]) & 0x0FFF]; 
		break;
		
		//ai
		case 9:
		op1 = (cinstruct[0] & 0x0F);
		op2 = cinstruct[1];
		break;
		
		//br
		case 10:
		case 11:
		op1 = regi[(cinstruct[0] & 0x0F)];
		op2 = regi[(cinstruct[1] >> 4) & 0x0F];
		offset = cinstruct[1];
		offset = offset << 16;
		temp = cinstruct[2];
		temp = temp << 8;
		offset = (offset | temp | cinstruct[3]) & 0x0FFFFF; //20 bits
		break;
		
		//jmp
		case 12:
		op1 = cinstruct[0];
		op1 = (op1 << 24);
		op2 = cinstruct[1];
		op2 = (op2 << 16);
		temp = cinstruct[2];
		temp = temp << 8;
		op1 = (op1 | op2 | temp | cinstruct[3]) & 0x0FFFFFFF; //28 bits
		break;
		
		//iter
		case 13:
		op1 = regi[(cinstruct[0] & 0x0F)];
		op2 = cinstruct[1]; // pointer offset
		offset = cinstruct[2];
		offset = offset << 8;
		offset = (offset | cinstruct[3]) & 0x0FFFF; //16 bits adress offset
		break;
		
		//ls
		case 14:
		case 15:
		op1 = regi[(cinstruct[0] & 0x0F)];
		op2 = regi[(cinstruct[1] >> 4) & 0x0F];
		offset = cinstruct[1] & 0x0F;
		if ((offset >> 3) == 0x01)
		{
			offset = offset | 0xFFFFFFF0;
		}
		break;
}
}

void execute(){
	switch(ophold)
	{
		case 0: //halt
		running = 0;
		pc = pc + 2;
		break;
		
		case 1: //add
		result = op1 + op2;
		pc = pc + 2;
		break;
		
		case 2: //and
		result = op1 & op2;
		pc = pc + 2;
		break;
		
		case 3: //divide
		result = op1 / op2;
		pc = pc + 2;
		break;
		
		case 4: //multiply
		result = op1 * op2;
		pc = pc + 2;
		break;
		
		case 5: //subtract
		result = op1 - op2;
		pc = pc + 2;
		break;
		
		case 6: //or
		result = op1 | op2;
		pc = pc + 2;
		break;

		case 7: //rightshift / leftshift
		if ((cinstruct[1] & 0x20) == 0x20)
		{
			result = op1 >> op2;
		}
		else
		{
			result = op1 << op2;
		}
		pc = pc + 2; 
		break;
		
		case 8: //interupt
		for (int i = 0; i < 16; i++)
		{
			printf("Register[%d] = %d \n", i, regi[i]);
		}
		pc = pc + 2; 
		break;
		
		case 9: //addimmediate 
		result = op2;
		pc = pc + 2;
		break;
		
		case 10: //branchifequal
		if (op1 == op2)
		{
			pc = pc + offset;
		}
		else
		{	
			pc = pc + 4;	
		}	
		break;
		
		case 11: //branchifless
		if (op1 < op2)
		{
			pc = pc + offset;
		}
		else
		{	
			pc = pc + 4;	
		}	
		break;
	
		case 12: //jump
		pc = pc + 4;
		pc = op1;
		break;
		
		case 13: //iterateover
		op1 = op1 + op2; // offset added to pointer 
		result = (mem[op1] << 24); // memory points to the last 8 bits and need to be shifted in order to save to result properly on the end
		result = result | (mem[op1+1] << 16); // memory pointer is moved up to the next 8 bits and needs to be shifted to bed the second byte from the left
		result = result | (mem[op1+2] << 8);// pointer is moved again 8 bits and needs to shifted and ored to get the 2nd byte from the right 
		result = result | (mem[op1+3]);    // finally the last bit at the left, does not need be shifted sense the pointer is already there and just needs to be ored.

		if ( result != 0) // checker so its not null
		{
			pc = pc - offset;
		}
		else 
		{
			pc = pc + 4;
		}
		break;
		
		case 14: //load
		result = op2 + offset;
		pc = pc + 2;
		break;
		
		case 15: //store
		result = op2 + offset;
		pc = pc + 2;
		break;
	}
		
		

}

void store(){
	
	switch(ophold){
		case 0: //halt
		break;
		
		case 1: //add
		case 2: //and
		case 3: //divide
		case 4: //multiply
		case 5: //subtract
		case 6: //or
		regi[(cinstruct[1] & 0x0F)] = result;
		break;

		case 7: //rightshift / leftshift
		regi[(cinstruct[1] & 0x0F)] = result;
		break;
		
		case 8: //interupt
		break;

		case 9: //addimmediate 
		regi[op1] = result;
		break;
		
		case 10: //branchifequal
		case 11: //branchifless
		break;
		
		case 12: //jump
		break;
		
		case 13: //iterateover
		regi[(cinstruct[0] & 0x0F)] = result; // store new value into the register 
		break;
		
		case 14: //load (dont think it works)
		regi[op1] = (mem[result] << 24);
		regi[op1] = (regi[op1] | (mem[result+1] << 16)); // kind of like iterate over but needs to be done in store, sense no free op register.
		regi[op1] = (regi[op1] | (mem[result+2] << 8));
		regi[op1] = (regi[op1] | (mem[result+3]));
		break;
		
		case 15: //store (dont think it works)
		mem[result] = (mem[result] >> 24) ;
		mem[result+1] = ((mem[result+1] >> 16)); // opposite of store.
		mem[result+2] = ((mem[result+2] >> 8));
		mem[result+3] = ((mem[result+3]));
		break;
	}
}








void run(){
	while (running){
		fetch();
		dispatch();
		execute();
		store();
	}
}

int main(int argc,char **argv){
	initiate(argv[1]);
	run();
}

