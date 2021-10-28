
/*
 * Description: This is an initial for an instruction
 * Return: an Instruction
 * */
struct Instruction* new_instr(){
	Instruction *instr=malloc(1*sizeof(Instruction));
	instr -> opcode = -1;
	instr -> first_type = -1;
	instr -> first_value = -1;
	instr -> second_type = -1;
	instr -> second_value = -1;
	return instr;
}

/*
 * Description: This is an initial for an function
 * Return: a Function
 * */
struct Function* new_func(){
	Function *func=malloc(1*sizeof(Function));
	func -> instruct_num = -1;
	func -> stack_num = 0;
	func -> func_label = -1;
	func -> instructions = NULL;
	return func;
}

/*
 * Description: This is an initial for an program
 * Return: a Program
 * */
struct Program* new_prog(){
	Program *prog=malloc(1*sizeof(Program));
	prog -> function_num = -1;
	prog -> functions = NULL;
	return prog;
}

/*
 * Description: This is an function for free the program, all the 
 * functions inside the program and all the instructions in those
 * functions.
 * */
void destroy(Program *program){
	for(int i = 0; i < program -> function_num; ++i){
		for(int j = 0; j < program -> functions[i] -> instruct_num; ++j){
			free(program -> functions[i] -> instructions[j]);
		}
		free(program -> functions[i] -> instructions);
		free(program -> functions[i]);
	}
	free(program -> functions);
	free(program);
}

/*
 * Description: This is an function for checking the opcode and return how
 * many values should be after it.
 * Return: How many values should be after this opcode.
 * */
int opcode_num(int opcode){
	switch (opcode){
        case 0: return 2;
        case 1:	return 1; 
        case 2:	return 0;
        case 3:	return 2;
        case 4:	return 2;
        case 5:	return 1;
        case 6:	return 1;
        case 7:	return 1;
        default:printf("error\n");
    }
    return -1;
}

/*
 * Description: This is an function for checking the type and return how
 * many bits the value need.
 * Return: How many bits the value need.
 * */
int type_num(int type){
	switch (type){
        case 0: return 8;
        case 1:	return 3; 
        case 2:	return 5;
        case 3:	return 5;
        default:printf("error\n");
    }
    return -1;
}

/*
 * Description: This is an function for checking the opcode and return the
 * String belong to it.
 * Return: The string belongs to this opcode.
 * */
char* opcode_string(int opcode){
	switch (opcode){
        case 0: return "MOV";
        case 1:	return "CAL"; 
        case 2:	return "RET";
        case 3:	return "REF";
        case 4:	return "ADD";
        case 5:	return "PRINT";
        case 6:	return "NOT";
        case 7:	return "EQU";
        default: return "";
    }
    return "";
}

/*
 * Description: This is an function for checking the type and return the
 * String belong to it.
 * Return: The string belongs to this type.
 * */
char* type_string(int type){
	switch (type){
        case 0: return "VAL";
        case 1:	return "REG"; 
        case 2:	return "STK";
        case 3:	return "PTR";
        default: return "";
    }
    return "";
}

/*
 * Description: This is an function for do the pow.
 * Return: the given pow of the base
 * */
int my_pow(int base, int pow){
	int ret = 1;
	if(pow == 0){
		return ret;
	}
	for(int i = 0; i < pow; ++i){
		ret *= base;
	}
	return ret;
}

/*
 * Description: This is an function for generate a 8 bits mask
 * base on the start and the end point.
 * Return: a mask, e.g: mask(0,5) will be 0b00011111, which is 31 
 * */
int mask(int start, int end){
	int ret = 0;
	for(int i = start; i < end; ++i)
	{
		ret += my_pow(2,i);
	}
	return ret;
}

/*
 * Description: This is bitwise with right shift for get the bits we want
 * Return: a decimal number represent those bit
 * */
int bitwise_right(int byte, int mask, int shift){
	return (byte & mask) >> shift;
}

/*
 * Description: This is bitwise with left shift for get the bits we want
 * Return: a decimal number represent those bit
 * */
int bitwise_left(int byte, int mask, int shift){
	return (byte & mask) << shift;
}

/*
 * Description: This is a function to store every instruction into a list
 * */
void instruction_set(Instruction *instr, Function *func, int i){
	if (i == 0){
		func -> instructions = malloc((func -> instruct_num)
			*sizeof(Instruction*));
	}
	func -> instructions[func -> instruct_num - i - 1] = instr;
}

/*
 * Description: This is a function to store every function into a list 
 * */
void function_set(Function *func, Program *program){
	program -> function_num +=1;
	if (program -> function_num ==1){
		program -> functions = malloc((program -> function_num)
			*sizeof(Function*));
	}else{
		program -> functions = realloc(program -> functions, 
			(program -> function_num)*sizeof(Function));
	}
	program -> functions[program -> function_num - 1] = func;
}

/*
 * Description: This is a function to read the byte and bitwise it to get 
 * the bits we want and return it in decimal. 
 * Return: the bits we want in decimal.
 * */
int read_bits(FILE * fd, int curr, int last){
	// in the same byte
	if((curr-1)/8 == last/8){
		fseek(fd,-((curr-1)/8 +1),SEEK_END);
		int ch  = fgetc(fd);
		if(curr % 8 == 0){
			return ch << (curr/8*8 - curr) >> last % 8;
		}else{
			return bitwise_right(ch, mask(last%8, curr%8), last%8);
		}
	}
	// crossing byte
	else{
		fseek(fd,-((curr-1)/8 +1),SEEK_END);
		int ch1  = fgetc(fd);
		int mid = (curr-1)/8*8;
		int ch2  = fgetc(fd);
		if(curr % 8 == 0){
			return bitwise_left(ch1, mask(mid%8, 8), mid - last) + 
				bitwise_right(ch2, mask(last%8, 8), 8 - mid + last);
		}else{
			return bitwise_left(ch1, mask(mid%8, curr%8), mid - last) + 
				bitwise_right(ch2, mask(last%8, 8), 8 - mid + last);
		}
	}
}

/*
 * Description: This is a function convert the int to letter
 * Return: a letter
 * */
int int_to_letter(int num){
	if(num + 'A' > 'Z' && num + 'A' < 'a'){
		return num + 'A' + 6;
	}else if(num +'A' >='a'){
		return -1;
	}else{
		return num+'A';
	}

}

/*
 * Description: This is a function format the stk and ptr to letter 
 * in function
 * */
void format_function_int_to_letter(int *stack, int *type, int *value, int* ch){
	if(*type == 2){
		if(stack[*value] == -1){
			stack[*value] = *ch;
			*ch +=1;
		}
		*value = stack[*value];
	}else if(*type == 3){
		if(stack[*value] != -1){
			*value = stack[*value];
		}else{
			printf("No matching stack for this ptr\n");
			exit(1);
		}                           
	}
}

/*
 * Description: This is a function format the function, mainly for stk
 * and ptr
 * */
void format_function(Program *program){
	int start_func = program -> function_num -1;
	while(start_func >=0){
		Function *func = program -> functions[start_func];
		int ch = 0;
		int *stack=malloc(sizeof(int)*32);
		for(int i = 0; i < 32; ++i){
			stack[i] = -1;
		}
		int start_instr = 0;
		while(start_instr < func -> instruct_num){
			Instruction *instr = func -> instructions[start_instr];
			format_function_int_to_letter(stack, &(instr -> first_type), 
				&(instr -> first_value), &ch);
			format_function_int_to_letter(stack, &(instr -> second_type), 
				&(instr -> second_value), &ch);
			start_instr +=1;
		}
		func -> stack_num = ch;
		free(stack);
		start_func -=1;
	}
}

/*
 * Description: This is a function read type and value inside a instruction
 * */
void read_instr_type_val(int *last, int *curr, FILE *fd, int *type, int *val,
 int length){
	if(length - *curr < 2){
		printf("No enough bit for type\n");
		exit(1);
	}
	*last = *curr;
	*curr = *curr + 2;
	*type = read_bits(fd, *curr, *last);
	if(length - *curr < type_num(*type)){
		printf("No enough bit for value\n");
		exit(1);
	}
	*last = *curr;
	*curr = *curr + type_num(*type);
	*val = read_bits(fd, *curr, *last);
}

/*
 * Description: This is a function read a instruction
 * */
void read_instr(int *last, int *curr,FILE * fd,Instruction *instr,int length){
	if(length - *curr < 3){
		printf("No enough instruction\n");
		exit(1);
	}
	*last = *curr;
	*curr = *curr + 3;
	instr -> opcode = read_bits(fd, *curr, *last);
	int value_n = opcode_num(instr -> opcode);
	if (value_n == 1){
		read_instr_type_val(&*last, &*curr, fd, &(instr -> first_type ), 
			&(instr -> first_value), length);
	}
	else if(value_n == 2){
		read_instr_type_val(&*last, &*curr, fd, &(instr -> first_type), 
			&(instr -> first_value), length);
		read_instr_type_val(&*last, &*curr, fd, &(instr -> second_type), 
			&(instr -> second_value), length);
	}
}

/*
 * Description: This is a function read a function
 * */
void read_func(int *last, int *curr, FILE * fd, Program* program, int length){
	Function *func = new_func();

	func -> instruct_num = read_bits(fd, *curr, *last);
	
	for (int i = 0;i < func -> instruct_num; ++i){
		Instruction *instr = new_instr();
		read_instr(&*last, &*curr, fd, instr, length);
		instruction_set(instr, func, i);
	}
	if(length - *curr < 3){
		printf("No function label\n");
		exit(1);
	}
	*last = *curr;
	*curr = *curr + 3;
	func -> func_label = read_bits(fd, *curr, *last);
	
	function_set(func, program);

}

/*
 * Description: This is a function read a program
 * */
struct Program* read_program(FILE * fd, int length){
	int last = 0;
    int curr = 5;

    Program* program = new_prog();
    program -> function_num = 0;

    while(1){
    	read_func(&last, &curr, fd, program, length);
		if(length - curr < 11){
			break;
		}
		last = curr;
		curr = curr + 5;
	}
	format_function(program);
	return program;
}

/*
 * Description: This is a function for read the whole binary file
 * if the function is null, throw an error
 * */
struct Program* read_binary_file(char *file){
	FILE * fd = fopen(file,"rb");
	if(fd == NULL){
		printf("Can not open the file\n");
		exit(1); 
	}
	fseek(fd, 0, SEEK_END);
	int length = ftell(fd) * 8;
	Program* program = read_program(fd, length);
	fclose(fd);
	return program;
}
