#ifndef _x2027_H_
#define _x2017_H_
/*
 * Description: This is a struct for each instruction
 * */
typedef struct Instruction{
	int opcode;
	int first_type;
	int first_value;
	int second_type;
	int second_value;
} Instruction;

/*
 * Description: This is a struct for each function
 * */
typedef struct Function{
	int instruct_num;
	int stack_num;
	int func_label;
	struct Instruction** instructions;
} Function;

/*
 * Description: This is a struct for the whole program
 * */
typedef struct Program{
	int function_num;
	struct Function** functions;
} Program;

struct Instruction* new_instr();
struct Function* new_func();
struct Program* new_prog();
void destroy(Program *program);
int opcode_num(int opcode);
int type_num(int type);
char* opcode_string(int opcode);
char* type_string(int type);
int my_pow(int base, int pow);
int mask(int start, int end);
int bitwise_right(int byte, int mask, int shift);
int bitwise_left(int byte, int mask, int shift);
void instruction_set(Instruction *instr, Function *func, int i);
void function_set(Function *func, Program *program);
int read_bits(FILE * fd, int curr, int last);
int int_to_letter(int num);
void format_function_int_to_letter(int *stack, int *type, int *value, int* ch);
void format_function(Program *program);
void read_instr_type_val(int *last, int *curr, FILE *fd, int *type, int *val, 
	int length);
void read_instr(int *last, int *curr,FILE * fd,Instruction *instr, int length);
void read_func(int *last, int *curr, FILE * fd, Program* program, int length);
struct Program* read_program(FILE * fd, int length);
struct Program* read_binary_file(char *file);

#include "x2017.c"

#endif