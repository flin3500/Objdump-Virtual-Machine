#include <stdlib.h>
#include <stdio.h>
#include "x2017.h"
/*
 * Description: This is a function for outputing the type and value
 * of a instruction
 * */
void print_command_type_value(int type, int value){
	if(type != -1){
		printf(" %s", type_string(type));
	}
	if(type == 2){
		printf(" %c", int_to_letter(value));
	}else if(type == 3){
		printf(" %c", int_to_letter(value));
	}else if(value != -1){
		printf(" %d", value);
	}
}

/*
 * Description: This is a function for outputing all the functions
 * */
void print_function(Program *program){
	int start_func = program -> function_num -1;
	while(start_func >=0){
		Function *func = program -> functions[start_func];
		printf("FUNC LABEL %d\n", func -> func_label);
		int start_instr = 0;
		while(start_instr < func -> instruct_num){
			Instruction *instr = func -> instructions[start_instr];
			printf("    %s", opcode_string(instr -> opcode));
			print_command_type_value(instr->first_type, instr->first_value);
			print_command_type_value(instr->second_type, instr->second_value);
			printf("\n");
			start_instr +=1;
		}
		start_func -=1;
	}
}

int main(int argc, char *argv[]){
	Program* program = read_binary_file(argv[1]);
	print_function(program);
	destroy(program);
	return 0;
}