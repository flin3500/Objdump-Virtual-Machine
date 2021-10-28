#include <stdlib.h>
#include <stdio.h>
#include "x2017.h"
/*
 * Description: This is a function ruturn the function by the
 * current program counter
 * Return: a function
 * */
struct Function* running_func(Program *program, int num){
	for(int i=0; i< program -> function_num; ++i)
	{
		if(program -> functions[i] -> func_label == num){
			return program -> functions[i];
		}
	}
	return NULL;
}

/*
 * Description: This is a function for equ opcode
 * */
void equ_opcode(unsigned char *reg, Instruction* instr){
	// printf("equ\n");
	if(instr -> first_type != 1){
		printf("equ should be reg");
		exit(1);
	}
	if(instr -> first_type ==1){
		if(reg[instr -> first_value] == 0){
			reg[instr -> first_value] = 1;
		}else{
			reg[instr -> first_value] = 0;
		}
	}
	reg[7]+=1;
}

/*
 * Description: This is a function for not opcode
 * */
void not_opcode(unsigned char *reg, Instruction* instr){
	// printf("not\n");
	if(instr -> first_type != 1){
		printf("not should be reg");
		exit(1);
	}
	if(instr -> first_type ==1){
		reg[instr -> first_value] = ~reg[instr -> first_value];
	}
	reg[7]+=1;
}

/*
 * Description: This is a function for print opcode
 * */
void print_opcode(unsigned char *reg, unsigned char *ram, Instruction* instr){
	if(instr -> first_type ==0){
		// printf("print val\n");
		printf("%d\n",instr -> first_value);
	}
	if(instr -> first_type ==1){
		// printf("print reg\n");
		printf("%d\n",reg[instr -> first_value]);
	}
	if(instr -> first_type ==2){
		// printf("print stk\n");
		printf("%d\n",ram[reg[5] + instr -> first_value]);
	}
	if(instr -> first_type ==3){
		// printf("print ptr\n");
		printf("%d\n",ram[ram[reg[5] + instr -> first_value]]);
	}
	reg[7]+=1;
}

/*
 * Description: This is a function for add opcode
 * */
void add_opcode(unsigned char *reg, Instruction* instr){
	// printf("add reg reg\n");
	if(instr -> first_type != 1){
		printf("add should be reg");
		exit(1);
	}
	if(instr -> second_type != 1){
		printf("add should be reg");
		exit(1);
	}
	reg[instr -> first_value] += reg[instr -> second_value];
	reg[7]+=1;
}

/*
 * Description: This is a function for ref opcode
 * */
void ref_opcode(unsigned char *reg, unsigned char *ram, Instruction* instr){
	if(instr -> first_type ==0){
		printf("Ref destination can not be val");
		exit(1);
	}
	if(instr -> first_type ==1){
		if(instr -> second_type ==0){
			printf("Ref source can not be val");
			exit(1);
		}
		if(instr -> second_type ==1){
			printf("Ref source can not be reg");
			exit(1);
		}
		if(instr -> second_type ==2){
			// printf("ref reg stk\n");
			reg[instr -> first_value] = reg[5] 
			+ instr -> second_value;
		}
		if(instr -> second_type ==3){
			// printf("ref reg ptr\n");
			reg[instr -> first_value] = ram[reg[5] 
				+ instr -> second_value];
		}
	}
	if(instr -> first_type ==2){
		if(instr -> second_type ==0){
			printf("Ref source can not be val");
			exit(1);
		}
		if(instr -> second_type ==1){
			printf("Ref source can not be reg");
			exit(1);
		}
		if(instr -> second_type ==2){
			// printf("ref stk stk\n");
			ram[reg[5] + instr -> first_value] = reg[5] 
			+ instr -> second_value;
		}
		if(instr -> second_type ==3){
			// printf("ref stk ptr\n");
			ram[reg[5] + instr -> first_value] = ram[reg[5] 
				+ instr -> second_value];
		}
	}
	if(instr -> first_type ==3){
		if(instr -> second_type ==0){
			printf("Ref source can not be val");
			exit(1);
		}
		if(instr -> second_type ==1){
			printf("Ref source can not be reg");
			exit(1);
		}
		if(instr -> second_type ==2){
			// printf("ref ptr stk\n");
			ram[ram[reg[5] + instr -> first_value]] = reg[5] 
			+ instr -> second_value;
		}
		if(instr -> second_type ==3){
			// printf("ref ptr ptr\n");
			ram[ram[reg[5] + instr -> first_value]] = ram[reg[5]
			 + instr -> second_value];
		}
	}
	reg[7]+=1;
}

/*
 * Description: This is a function for ret opcode
 * */
void ret_opcode(unsigned char *reg, unsigned char *ram, 
	Program *program,Function* func){
	// printf("ret\n");
	reg[7] = ram[reg[5]-1];
	reg[5] = ram[reg[5]-2];
	func = running_func(program, reg[7] >> 5);
	reg[6] = reg[5] + func -> stack_num;
}

/*
 * Description: This is a function for cal opcode
 * */
void cal_opcode(unsigned char *reg, unsigned char *ram,
 Instruction* instr, Program *program, Function* func){
	// printf("cal\n");
	ram[reg[6]] = reg[5];
	ram[reg[6]+1] = reg[7] + 1;
	reg[5] = reg[6] + 2;
	reg[7] = instr -> first_value << 5;
	func = running_func(program, reg[7] >> 5);
	reg[6] = reg[5] + func -> stack_num;
	if (reg[6] > 255){
		printf("stack overflow\n");
		exit(1);
	}
}

/*
 * Description: This is a function for mov opcode
 * */
void mov_opcode(unsigned char *reg, unsigned char *ram, 
	Instruction* instr){
	if (instr -> first_type == 0){
		printf("Move destination can not be val");
		exit(1);
	}
	if (instr -> first_type == 1){
		if (instr -> second_type == 0){
			// printf("mov reg val\n");
			reg[instr -> first_value] = instr -> second_value;
		}
		if (instr -> second_type == 1){
			// printf("mov reg reg\n");
			reg[instr -> first_value] = reg[instr -> second_value];
		}
		if (instr -> second_type == 2){
			// printf("mov reg stk\n");
			reg[instr -> first_value] = ram[reg[5] 
				+ instr -> second_value];
		}
		if (instr -> second_type == 3){
			// printf("mov reg ptr\n");
			reg[instr -> first_value] = ram[ram[reg[5] 
				+ instr -> second_value]];
		}
	}
	if (instr -> first_type == 2){
		if (instr -> second_type == 0){
			// printf("mov stk val\n");
			ram[reg[5] + instr -> first_value] = instr -> second_value;
		}
		if (instr -> second_type == 1){
			// printf("mov stk reg\n");
			ram[reg[5] + instr -> first_value] = reg[instr -> second_value];

		}
		if (instr -> second_type == 2){
			// printf("mov stk stk\n");
			ram[reg[5] + instr -> first_value] = ram[reg[5] 
				+ instr -> second_value];
		}
		if (instr -> second_type == 3){
			// printf("mov stk ptr\n");
			ram[reg[5] + instr -> first_value] = ram[ram[reg[5] 
				+ instr -> second_value]];
		}
	}
	if (instr -> first_type == 3){
		if (instr -> second_type == 0){
			// printf("mov ptr val\n");
			ram[ram[reg[5] + instr -> first_value]] = 
			instr -> second_value;
		}
		if (instr -> second_type == 1){
			// printf("mov ptr reg\n");
			ram[ram[reg[5] + instr -> first_value]] = 
			reg[instr -> second_value];	
		}
		if (instr -> second_type == 2){
			// printf("mov ptr stk\n");
			ram[ram[reg[5] + instr -> first_value]] = ram[reg[5] 
				+ instr -> second_value];
		}
		if (instr -> second_type == 3){
			// printf("mov ptr ptr\n");
			ram[ram[reg[5] + instr -> first_value]] = ram[ram[reg[5] 
				+ instr -> second_value]];
		}
	}
	reg[7]+=1;
}

/*
 * Description: This is a function for running the program
 * */
void run_program(Program *program){
	unsigned char *ram = malloc(sizeof(unsigned char)*256);
	unsigned char *reg = malloc(sizeof(unsigned char)*8);

	reg[7] = 0; // program counter

	Function* func = running_func(program, reg[7] >> 5);
	if (func == NULL && reg[7] >> 5 == 0){
		printf("no entry\n");
		exit(1);
	}else if(func == NULL){
		printf("Function not exists\n");
		exit(1);
	}
	
	reg[6] = 2 + func -> stack_num; // ram head
	reg[5] = 2; // stack frame start
	reg[4] = 0; // keep the program counter for last
	ram[0] = 0 << 5;
	ram[1] = 2;

	Instruction* instr = func -> instructions[reg[7] & 31];
	while(1){
		if(instr -> opcode == 7){
			equ_opcode(reg, instr);
		}
		if(instr -> opcode == 6){
			not_opcode(reg, instr);
		}
		if(instr -> opcode == 5){
			print_opcode(reg, ram, instr);
		}
		if(instr -> opcode == 4){
			add_opcode(reg, instr);
		}
		if(instr -> opcode == 3){
			ref_opcode(reg, ram, instr);
		}
		if(instr -> opcode == 2){
			if(reg[7] >> 5 == 0){break;}
			ret_opcode(reg, ram, program, func);
		}
		if(instr -> opcode == 1){
			cal_opcode(reg, ram, instr, program, func);
		}
		if(instr -> opcode == 0){
			mov_opcode(reg, ram, instr);
		}
		func = running_func(program, reg[7] >> 5);
		instr = func -> instructions[reg[7] & 31];
	}
	free(reg);
	free(ram);
}

int main(int argc, char *argv[]){
	Program* program = read_binary_file(argv[1]);
	run_program(program);
	destroy(program);
	return 0;
}