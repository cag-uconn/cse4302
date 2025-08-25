#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <unistd.h>
#include "file_parser.h"
#include "tokenizer.h"

int search(char *instruction);

/*
 * The structs below map a character to an integer.
 * They are used in order to map a specific instruciton/register to its binary format in ASCII
 */

int incr = 0;

// Struct that stores registers and their respective binary reference
//Fall 2023 update: changes for RISC-V ISA
struct {
	const char *reg;
	const char *name;
	char *address;
} registerMap[] = {
	{ "x0", "zero", "00000" },
	{ "x1", "ra",   "00001" },
	{ "x2", "sp",   "00010" },
	{ "x3", "gp",   "00011" }, 
	{ "x4", "tp",   "00100" }, 
	{ "x5", "t0",   "00101" },
	{ "x6", "t1",   "00110" },
	{ "x7", "t2",   "00111" },
	{ "x8", "s0",   "01000" },
	{ "x9", "s1",   "01001" },
	{ "x10", "a0",  "01010" },
	{ "x11", "a1",  "01011" },
	{ "x12", "a2",  "01100" },
	{ "x13", "a3",  "01101" },
	{ "x14", "a4",  "01110" },
	{ "x15", "a5",  "01111" },
	{ "x16", "a6",  "10000" },
	{ "x17", "a7",  "10001" },
	{ "x18", "s2",  "10010" },
	{ "x19", "s3",  "10011" },
	{ "x20", "s4",  "10100" },
	{ "x21", "s5",  "10101" },
	{ "x22", "s6",  "10110" },
	{ "x23", "s7",  "10111" },
	{ "x24", "s8",  "11000" },
	{ "x25", "s9",  "11001" },
	{ "x26", "s10", "11010" },
	{ "x27", "s11", "11011" },
	{ "x28", "t3",  "11100" },
	{ "x29", "t4",  "11101" },
	{ "x30", "t5",  "11110" },
	{ "x31", "t6",  "11111" },
	{ NULL,  NULL,  0 } 
};

// Struct for R-Type instructions mapping for the 'function' field in the instruction
struct {
	const char *name;
	char *f7;
	char *f3;
	char *op;
} rMap[] = {
	{ "add", "0000000", "000", "0110011" },
	{ "sub", "0100000", "000", "0110011" },
	{ "slt", "0000000", "010", "0110011" },
	{ "sll", "0000000", "001", "0110011" },
	{ "srl", "0000000", "101", "0110011" },
	{ "and", "0000000", "111", "0110011" },
	{ "or",  "0000000", "110", "0110011" },
	{ "xor", "0000000", "100", "0110011" },
	{ NULL,  NULL,      NULL,  NULL      } 
};

// Struct for I-Type instructions
struct {
	const char *name;
	char *f3;
	char *op;
} iMap[] = {
	{ "jalr", "000", "1100111" },
	{ "addi", "000", "0010011" },
	{ "slti", "010", "0010011" },
	{ "andi", "111", "0010011" },
	{ "ori",  "110", "0010011" },
	{ "xori", "100", "0010011" },
	{ "slli", "001", "0010011" },
	{ "srli", "101", "0010011" },
	{ "lw",   "010", "0000011" },
	{ NULL,   0,     0 } 
};

// Struct for B-Type instructions
struct {
	const char *name;
	char *f3;
	char *op;
} bMap[] = {
	{ "beq", "000", "1100011" },
	{ "bne", "001", "1100011" },
	{ "blt", "100", "1100011" },
	{ "bge", "101", "1100011" },
	{ NULL, 0, 0 } 
};

//Struct for S-type instructions
struct {
	const char *name;
	char *f3;
	char *op;
} sMap[] = {
	{ "sw", "010", "0100011" },
	{ NULL, 0, 0}
};


// Struct for J-Type instructions
struct {
	const char *name;
	char *op;
} jMap[] = {
	{ "jal", "1101111" },
	{ NULL,  0 } 
};

// Struct for U-Type instructions
struct {
	const char *name;
	char *op;
} uMap[] = {
	{ "lui", "0110111" },
	{ NULL,  0 } 
};

int memory_location = 0;

void parse_file(FILE *fptr, int pass, char *instructions[], size_t inst_len, hash_table_t *hash_table, FILE *Out) {
	
	char line[MAX_LINE_LENGTH + 1];
	char *tok_ptr, *ret, *token = NULL;
	int32_t line_num = 1;
	int32_t instruction_count = 0x00000000;
	int data_reached = 0;

	while (1) {
		if ((ret = fgets(line, MAX_LINE_LENGTH, fptr)) == NULL) break;
		line[MAX_LINE_LENGTH] = 0;
		tok_ptr = line;
		if (strlen(line) == MAX_LINE_LENGTH) {
			fprintf(Out, "line %d: line is too long. ignoring line ...\n", line_num);
			line_num++;
			continue;
		}

		/* parse the tokens within a line */
		while (1) {
			token = parse_token(tok_ptr, " \n\t$,()", &tok_ptr, NULL);

			/* blank line or comment begins here. go to the next line */
			if (token == NULL || *token == '#') {
				line_num++;
				free(token);
				break;
			}

			printf("token: %s\n", token);

			/*
			 * If token is "la", increment by 8, otherwise if it exists in instructions[],
			 * increment by 4.
			 */
			int x = search(token);
			if (x >= 0) {
				if (strcmp(token, "la") == 0)
					instruction_count = instruction_count + 8;
				else
					instruction_count = instruction_count + 4;
			}

			// If token is ".data", reset instruction to .data starting address
			else if (strcmp(token, ".data") == 0) {
                if(incr==1) fprintf(Out, "11111111111111111111111111111111\n");
                incr++;
				instruction_count = 0x00002000;
				data_reached = 1;
			}

			printf("PC Count: %d\n", instruction_count);

			// If first pass, then add labels to hash table
			if (pass == 1) {

				printf("First pass\n");

				// if token has ':', then it is a label so add it to hash table
				if (strstr(token, ":") && data_reached == 0) {

					printf("Label\n");

					// Strip out ':'
					//printf("Label: %s at %d with address %d: \n", token, line_num, instruction_count);
					size_t token_len = strlen(token);
					token[token_len - 1] = '\0';

					// Insert variable to hash table
					uint32_t *inst_count;
					inst_count = (uint32_t *)malloc(sizeof(uint32_t));
					*inst_count = instruction_count;
					int32_t insert = hash_insert(hash_table, token, strlen(token)+1, inst_count);

					if (insert != 1) {
						fprintf(Out, "Error inserting into hash table\n");
						exit(1);
					}
				}

				// If .data has been reached, increment instruction count accordingly
				// and store to hash table
				else {

					char *var_tok = NULL;
					char *var_tok_ptr = tok_ptr;

					// If variable is .word
					if (strstr(tok_ptr, ".word")) {

						printf(".word\n");

						// Variable is array
						if (strstr(var_tok_ptr, ":")) {

							printf("array\n");

							// Store the number in var_tok and the occurance in var_tok_ptr
							var_tok = parse_token(var_tok_ptr, ":", &var_tok_ptr, NULL);

							// Convert char* to int
							int freq = atoi(var_tok_ptr);

							int num;
							sscanf(var_tok, "%*s %d", &num);

							// Increment instruction count by freq
							instruction_count = instruction_count + (freq * 4);

							// Strip out ':' from token
							size_t token_len = strlen(token);
							token[token_len - 1] = '\0';

							//printf("Key: '%s', len: %zd\n", token, strlen(token));

							// Insert variable to hash table
							uint32_t *inst_count;
							inst_count = (uint32_t *)malloc(sizeof(uint32_t));
							*inst_count = instruction_count;
							int32_t insert = hash_insert(hash_table, token, strlen(token)+1, inst_count);

							if (insert == 0) {
								fprintf(Out, "Error in hash table insertion\n");
								exit(1);
							}

							printf("End array\n");
						}

						// Variable is a single variable
						else {

							instruction_count = instruction_count + 4;

							// Strip out ':' from token
							size_t token_len = strlen(token);
							token[token_len - 1] = '\0';

							// Insert variable to hash table
							uint32_t *inst_count;
							inst_count = (uint32_t *)malloc(sizeof(uint32_t));
							*inst_count = instruction_count;
							int32_t insert = hash_insert(hash_table, token, strlen(token)+1, inst_count);

							if (insert == 0) {
								fprintf(Out, "Error in hash table insertion\n");
								exit(1);
							}

							printf("end singe var\n");
						}
					}

					// Variable is a string
					else if (strstr(tok_ptr, ".asciiz")) {

						// Store the ascii in var_tok
						var_tok_ptr+= 8;
						var_tok = parse_token(var_tok_ptr, "\"", &var_tok_ptr, NULL);

						// Increment instruction count by string length
						size_t str_byte_len = strlen(var_tok);
						instruction_count = instruction_count + str_byte_len;

						// Strip out ':' from token
						size_t token_len = strlen(token);
						token[token_len - 1] = '\0';

						// Insert variable to hash table
						uint32_t *inst_count;
						inst_count = (uint32_t *)malloc(sizeof(uint32_t));
						*inst_count = instruction_count;
						int32_t insert = hash_insert(hash_table, token, strlen(token)+1, inst_count);

						if (insert == 0) {
							fprintf(Out, "Error in hash table insertion\n");
							exit(1);
						}
					}
				}
			}
			// If second pass, then interpret
			
			else if (pass == 2) {

				printf("############    Pass 2   ##############\n");

				// start interpreting here
				// if j loop --> then instruction is: 000010 then immediate is insturction count in 26 bits??

				// If in .text section
				if (data_reached == 0) {

					// Check instruction type
					int instruction_supported = search(token);
					char inst_type;

					// If instruction is supported
					if (instruction_supported != -1) {

						// token contains the instruction
						// tok_ptr points to the rest of the line

						// Determine instruction type
						inst_type = instruction_type(token);


						if (inst_type == 'r') {

							// Parse the instructio - get rd, rs, rt registers
							char *inst_ptr = tok_ptr;
							char *reg = NULL;

							// Create an array of char* that stores rd, rs, rt respectively
							char **reg_store;
							reg_store = malloc(3 * sizeof(char*));
							if (reg_store == NULL) {
								fprintf(Out, "Out of memory\n");
								exit(1);
							}

							for (int i = 0; i < 3; i++) {
								reg_store[i] = malloc(2 * sizeof(char));
								if (reg_store[i] == NULL) {
									fprintf(Out, "Out of memory\n");
									exit(1);
								}
							}

							// Keeps a reference to which register has been parsed for storage
							int count = 0;
							while (1) {

								reg = parse_token(inst_ptr, " $,\n\t()", &inst_ptr, NULL);

								if (reg == NULL || *reg == '#') {
									break;
								}

								strcpy(reg_store[count], reg);
								count++;
								free(reg);
							}

							// Send reg_store for output
							// rd is in position 0, rs1 is in position 1 and rs2 is in position 2
							rtype_instruction(token, reg_store[1], reg_store[2], reg_store[0], Out);

							// Dealloc reg_store
							for (int i = 0; i < 3; i++) {
								free(reg_store[i]);
							}
							free(reg_store);

						// I-Type or S-type
						} else if ((inst_type == 'i') || (inst_type == 's')) {
							// Parse the instruction - rt, rs, immediate
							char *inst_ptr = tok_ptr;
							char *reg = NULL;

							// Create an array of char* that stores rt, rs, immediate
							char **reg_store;
							reg_store = malloc(3 * sizeof(char*));
							if (reg_store == NULL) {
								fprintf(Out, "Out of memory\n");
								exit(1);
							}

							for (int i = 0; i < 3; i++) {
								reg_store[i] = malloc(3 * sizeof(char));
								if (reg_store[i] == NULL) {
									fprintf(Out, "Out of memory\n");
									exit(1);
								}
							}

							// Keeps a reference to which register has been parsed for storage
							int count = 0;
							while (1) {

								reg = parse_token(inst_ptr, " $,\n\t()", &inst_ptr, NULL);

								if (reg == NULL || *reg == '#') {
									break;
								}

								strcpy(reg_store[count], reg);
								count++;
								free(reg);
							}

							int immediate;
							//Handle i
							if (inst_type == 'i'){
								// rd in position 0, immediate in position 1, and rs1 in position 2 for lw
								if (strcmp(token, "lw") == 0){
									immediate = strtol(reg_store[1], NULL, 0);
									itype_instruction(token, reg_store[2], reg_store[0], immediate, Out);
								//Other i type have rd position 0, rs1 position 1, immediate position 2
								}else{
									immediate = strtol(reg_store[2], NULL, 0);
									itype_instruction(token, reg_store[1], reg_store[0], immediate, Out);
								}

							//Handle s
							}else{
								immediate = strtol(reg_store[1], NULL, 0);
								stype_instruction(token, reg_store[2], reg_store[0], immediate, Out);
							}

							// Dealloc reg_store
							for (int i = 0; i < 3; i++) {
								free(reg_store[i]);
							}
							free(reg_store);

						//B-Type	
						} else if (inst_type == 'b') {
							// Parse the instruction - rt, rs, immediate
							char *inst_ptr = tok_ptr;
							char *reg = NULL;

							// Create an array of char* that stores rt, rs
							char **reg_store;
							reg_store = malloc(2 * sizeof(char*));
							if (reg_store == NULL) {
								fprintf(Out, "Out of memory\n");
								exit(1);
							}

							for (int i = 0; i < 2; i++) {
								reg_store[i] = malloc(3 * sizeof(char));
								if (reg_store[i] == NULL) {
									fprintf(Out, "Out of memory\n");
									exit(1);
								}
							}

							// Keeps a reference to which register has been parsed for storage
							int count = 0;
							while (1) {

								reg = parse_token(inst_ptr, " $,\n\t()", &inst_ptr, NULL);

								if (reg == NULL || *reg == '#') {
									break;
								}

								strcpy(reg_store[count], reg);
								count++;
								free(reg);

								if (count == 2) break;

							}

							//Getting the label
							reg = parse_token(inst_ptr, " $,\n\t()", &inst_ptr, NULL);
							printf("label is %s\n", reg);

							// Find hash address for a register and put in an immediate
							int *address = hash_find(hash_table, reg, strlen(reg)+1);

							// rs1 in position 0, rs2 in position 1
							int immediate = (*address - instruction_count + 4) >> 1; //What in tarnation?
							btype_instruction(token, reg_store[0], reg_store[1], immediate, Out);

							// Dealloc reg_store
							for (int i = 0; i < 2; i++) {
								free(reg_store[i]);
							}
							free(reg_store);
						
						
						} else if (inst_type == 'j') {
							// Parse the instruction - rd, immediate
							char *inst_ptr = tok_ptr;
							char *reg = NULL, *label = NULL;

							//Get rd
							reg = parse_token(inst_ptr, " $,\n\t()", &inst_ptr, NULL);

							//Get label
							label = parse_token(inst_ptr, " $,\n\t()", &inst_ptr, NULL);
							printf("reg is %s\n", reg);
							printf("label is %s\n", label);

							
							// Find hash address for a register and put in an immediate
							int *address = hash_find(hash_table, label, strlen(label)+1);

							int immediate = (*address - instruction_count + 4) >> 1; //What in tarnation?
							printf("\n %d %d\n", immediate, instruction_count);

							jtype_instruction(token, reg, immediate, Out);

							//Free memory
							free(reg);
							free(label);

						} else if (inst_type == 'u') {
							// Parse the instruction - rd, immediate
							char *inst_ptr = tok_ptr;
							char *reg = NULL;

							// Create an array of char* that stores rd, immediate
							char **reg_store;
							reg_store = malloc(2 * sizeof(char*));
							if (reg_store == NULL) {
								fprintf(Out, "Out of memory\n");
								exit(1);
							}

							for (int i = 0; i < 2; i++) {
								reg_store[i] = malloc(2 * sizeof(char));
								if (reg_store[i] == NULL) {
									fprintf(Out, "Out of memory\n");
									exit(1);
								}
							}

							// Keeps a reference to which register has been parsed for storage
							int count = 0;
							while (1) {

								reg = parse_token(inst_ptr, " $,\n\t()", &inst_ptr, NULL);

								if (reg == NULL || *reg == '#') {
									break;
								}

								strcpy(reg_store[count], reg);
								count++;
								free(reg);
							}

							// rd in position 0, immediate in position 1
							int immediate = strtol(reg_store[1], NULL, 0);
							utype_instruction(token, reg_store[0], immediate, Out);

							// Dealloc reg_store
							for (int i = 0; i < 2; i++) {
								free(reg_store[i]);
							}
							free(reg_store);
						}

						if (strcmp(token, "nop") == 0) {
							fprintf(Out, "00000000000000000000000000000000\n");
						}
					}
				}

				// If .data part reached
				else {

					char *var_tok = NULL;
					char *var_tok_ptr = tok_ptr;

					// If variable is .word
					if (strstr(tok_ptr, ".word")) {

						int var_value;

						// Variable is array
						if (strstr(var_tok_ptr, ":")) {
							// Store the number in var_tok and the occurance in var_tok_ptr
							var_tok = parse_token(var_tok_ptr, ":", &var_tok_ptr, NULL);

							// Extract array size, or variable frequency
							int freq = atoi(var_tok_ptr);

							// Extract variable value
							sscanf(var_tok, "%*s %d", &var_value);

							// Value var_value is repeated freq times. Send to binary rep function
							for (int i = 0; i < freq; i++) {
								word_rep(var_value, Out);
							}
						}

						// Variable is a single variable
						else {
							printf("\n%s\n ", var_tok_ptr);
							char * pch;
							pch = strtok (var_tok_ptr," ");
							pch = strtok (NULL, " ");
							var_value = strtol(pch, NULL, 0);
							printf ("\n INTGER: %d\n",var_value);
							word_rep(var_value, Out);
                                            
						}
					}

					// Variable is a string
					else if (strstr(tok_ptr, ".asciiz")) {

						printf("tok_ptr '%s'\n", tok_ptr);

						if (strncmp(".asciiz ", var_tok_ptr, 8) == 0) {

							// Move var_tok_ptr to beginning of string
							var_tok_ptr = var_tok_ptr + 9;

							// Strip out quotation at the end
							// Place string in var_tok
							var_tok = parse_token(var_tok_ptr, "\"", &var_tok_ptr, NULL);

							ascii_rep(var_tok, Out);
						}
					}
				}
			}
			free(token);
		}
	}
}

// Binary Search the Array
int binarySearch(char *instructions[], int low, int high, char *string) {

	int mid = low + (high - low) / 2;
	int comp = strcmp(instructions[mid], string);\

	if (comp == 0)
		return mid;

	// Not found
	if (high <= low)
		return -1;

	// If instructions[mid] is less than string
	else if (comp > 0)
		return binarySearch(instructions, low, mid - 1, string);

	// If instructions[mid] is larger than string
	else if (comp < 0)
		return binarySearch(instructions, mid + 1, high, string);

	// Return position
	else
		return mid;

	// Error
	return -2;
}

// Determine Instruction Type
char instruction_type(char *instruction) {

	if (strcmp(instruction, "add") == 0 || 
		strcmp(instruction, "sub") == 0 || 
		strcmp(instruction, "sll") == 0 || 
		strcmp(instruction, "srl") == 0 || 
		strcmp(instruction, "and") == 0 || 
		strcmp(instruction, "or")  == 0 || 
		strcmp(instruction, "xor") == 0 ||
		strcmp(instruction, "slt") == 0) return 'r';

	else if (strcmp(instruction, "jalr") == 0 || 
			 strcmp(instruction, "addi") == 0 || 
			 strcmp(instruction, "slti") == 0 || 
			 strcmp(instruction, "andi") == 0 || 
			 strcmp(instruction, "ori")  == 0 || 
			 strcmp(instruction, "xori") == 0 || 
			 strcmp(instruction, "slli") == 0 || 
			 strcmp(instruction, "srli") == 0 || 
			 strcmp(instruction, "lw")   == 0 ) return 'i';

	else if (strcmp(instruction, "beq") == 0 || 
	         strcmp(instruction, "bne") == 0 ||  
			 strcmp(instruction, "blt") == 0 ||
			 strcmp(instruction, "bge") == 0 ) return 'b';
	
	else if (strcmp(instruction, "sw") == 0) return 's';

	else if (strcmp(instruction, "jal") == 0) return 'j';

	else if (strcmp(instruction, "lui") == 0) return 'u';
	
	// Failsafe return statement
	return 0;
}

// Return the binary representation of the register
char *register_address(char *registerName) {

	size_t i;
	//Fall 2023 update: Now also check for the register name
	for (i = 0; registerMap[i].name != NULL; i++) {
		if ( (strcmp(registerName, registerMap[i].name) == 0) || (strcmp(registerName, registerMap[i].reg) == 0) ) {
			return registerMap[i].address;
		}
	}

	return NULL;
}

// Write out the R-Type instruction
void rtype_instruction(char *instruction, char *rs1, char *rs2, char *rd, FILE *Out) {

	//Stores the opcode and func bits
	char *opcode = NULL, *f3 = NULL, *f7 = NULL;
	for (int i = 0; rMap[i].name != NULL; i++) {
		if (strcmp(instruction, rMap[i].name) == 0) {
			opcode = rMap[i].op;
			f3 = rMap[i].f3;
			f7 = rMap[i].f7;
			break;
		}
	}

	//Stores the binary representation of registers
	char *rdBin, *rs1Bin, *rs2Bin;
	rdBin = register_address(rd);
	rs1Bin = register_address(rs1);
	rs2Bin = register_address(rs2);
	
	// Print out the instruction to the file
	fprintf(Out, "%s%s%s%s%s%s\n", f7, rs2Bin, rs1Bin, f3, rdBin, opcode);
}

// Write out the I-Type instruction
void itype_instruction(char *instruction, char *rs1, char *rd, int immediate, FILE *Out) {

	int imm = immediate;
	
	//Stores the opcode and func bits
	char *opcode = NULL, *f3 = NULL;
	for (int i = 0; iMap[i].name != NULL; i++) {
		if (strcmp(instruction, iMap[i].name) == 0) {
			opcode = iMap[i].op;
			f3 = iMap[i].f3;
			break;
		}
	}

	//Clear the upper 7 immediate bits for SLLI or SRLI
	/*
	if ( strcmp(instruction, "slli") == 0 ||
		 strcmp(instruction, "slri") == 0 ) imm &= 0x01FFFFFF;
	*/
	// Set the instruction bits
	char *rs1Bin, *rdBin;
	rs1Bin = register_address(rs1);
	rdBin  = register_address(rd);

	// Convert immediate to binary string
	char iimmediate[13];
	getBin(imm, iimmediate, 12);
	printf("%s\n", iimmediate);

	// Print out the instruction to the file
	fprintf(Out, "%s%s%s%s%s\n", iimmediate, rs1Bin, f3, rdBin, opcode);
}



// Write out the I-Type instruction
void stype_instruction(char *instruction, char *rs1, char *rs2, int immediate, FILE *Out) {
	
	//Stores the opcode and func bits
	char *opcode = NULL, *f3 = NULL;
	for (int i = 0; sMap[i].name != NULL; i++) {
		if (strcmp(instruction, sMap[i].name) == 0) {
			opcode = sMap[i].op;
			f3 = sMap[i].f3;
			break;
		}
	}

	// Set the instruction bits
	char *rs1Bin, *rs2Bin;
	rs1Bin = register_address(rs1);
	rs2Bin = register_address(rs2);

	//B-type instructions break up immediates weirdly
	int upperimm_bits, lowerimm_bits;
	upperimm_bits = ((immediate & 0xFE0) >> 5);
	lowerimm_bits = immediate & 0x01F;

	// Convert immediate to binary string
	char immediate_upper[8], immediate_lower[6];
	getBin(upperimm_bits, immediate_upper, 7);
	getBin(lowerimm_bits, immediate_lower, 5);

	// Print out the instruction to the file
	fprintf(Out, "%s%s%s%s%s%s\n", immediate_upper, rs2Bin, rs1Bin, f3, immediate_lower, opcode);
}

// Write out the I-Type instruction
void btype_instruction(char *instruction, char *rs1, char *rs2, int immediate, FILE *Out) {
	
	//Stores the opcode and func bits
	char *opcode = NULL, *f3 = NULL;
	for (int i = 0; bMap[i].name != NULL; i++) {
		if (strcmp(instruction, bMap[i].name) == 0) {
			opcode = bMap[i].op;
			f3 = bMap[i].f3;
			break;
		}
	}

	// Set the instruction bits
	char *rs1Bin, *rs2Bin;
	rs1Bin = register_address(rs1);
	rs2Bin = register_address(rs2);

	//B-type instructions break up immediates weirdly
	int upperimm_bits, lowerimm_bits;
	upperimm_bits = ((immediate & 0x800) >> 5) | 
	 				 (immediate & 0x3F0) >> 4;

	lowerimm_bits = ((immediate & 0x400) >> 10) | 
	 				((immediate & 0x00F) << 1);
	
	// Convert immediate to binary string
	char immediate_upper[8], immediate_lower[6];
	getBin(upperimm_bits, immediate_upper, 7);
	getBin(lowerimm_bits, immediate_lower, 5);

	// Print out the instruction to the file
	fprintf(Out, "%s%s%s%s%s%s\n", immediate_upper, rs2Bin, rs1Bin, f3, immediate_lower, opcode);
}

// Write out the J-Type instruction
void utype_instruction(char *instruction, char *rd, int immediate, FILE *Out) {

	// Set the instruction bits
	char *opcode = NULL;

	// Get opcode bits
	size_t i;
	for (i = 0; uMap[i].name != NULL; i++) {
		if (strcmp(instruction, uMap[i].name) == 0) {
			opcode = uMap[i].op;
		}
	}

	char *rdBin;
	rdBin = register_address(rd);

	// Convert immediate to binary
	char immediateStr[21];
	getBin(immediate, immediateStr, 20);

	// Print out instruction to file
	fprintf(Out, "%s%s%s\n", immediateStr, rdBin, opcode);
}

// Write out the J-Type instruction
void jtype_instruction(char *instruction, char *rd, int immediate, FILE *Out) {

	// Set the instruction bits
	char *opcode = NULL;

	// Get opcode bits
	size_t i;
	for (i = 0; jMap[i].name != NULL; i++) {
		if (strcmp(instruction, jMap[i].name) == 0) {
			opcode = jMap[i].op;
		}
	}

	char *rdBin;
	rdBin = register_address(rd);
	int jump_bits = (immediate & 0x80000) | 
	 				((immediate & 0x003FF) << 9) |
					((immediate & 0x00400) >> 2) |
					((immediate & 0x7F800) >> 11);

	// Convert immediate to binary
	char immediateStr[21];
	getBin(jump_bits, immediateStr, 20);

	// Print out instruction to file
	fprintf(Out, "%s%s%s\n", immediateStr, rdBin, opcode);
}

// Write out the variable in binary
void word_rep(int binary_rep, FILE *Out) {

	for (int k = 31; k >= 0; k--) {
		fprintf(Out, "%c", (binary_rep & (1 << k)) ? '1' : '0');
	}

	fprintf(Out, "\n");
}

// Write out the ascii string
void ascii_rep(char string[], FILE *Out) {

	// Separate the string, and put each four characters in an element of an array of strings
	size_t str_length = strlen(string);
	str_length++;
	int num_strs = str_length / 4;
	if ((str_length % 4) > 0)
		num_strs++;

	char *ptr;
	ptr = &string[0];

	// Create an array of strings which separates each 4-char string
	char **sep_str;
	sep_str = malloc(num_strs * sizeof(char*));
	if (sep_str == NULL) {
		fprintf(Out, "Out of memory\n");
		exit(1);
	}

	for (int i = 0; i < num_strs; i++) {
		sep_str[i] = malloc(4 * sizeof(char));
		if (sep_str[i] == NULL) {
			fprintf(Out, "Out of memory\n");
			exit(1);
		}
	}

	int count = 0;
	for (int i = 0; i < str_length; i++) {
		sep_str[i / 4][i % 4] = *ptr;
		ptr++;
		count++;
	}

	// Reverse each element in the array
	char temp;

	for (int i = 0; i < num_strs; i++) {

		for (int j = 0, k = 3; j < k; j++, k--) {

			temp = sep_str[i][j];
			sep_str[i][j] = sep_str[i][k];
			sep_str[i][k] = temp;
		}
	}

	// Convert into binary
	for (int i = 0; i < num_strs; i++) {

		for (int j = 0; j < 4; j++) {
			char c = sep_str[i][j];
			for (int k = 7; k >= 0; k--) {
				fprintf(Out, "%c", (c & (1 << k)) ? '1' : '0');
			}
		}

		fprintf(Out, "\n");
	}

	// Deallocate sep_str
	for (int i = 0; i < num_strs; i++) {
		free(sep_str[i]);
	}
	free(sep_str);
	sep_str = NULL;
}

void getBin(int num, char *str, int size) {
	int n = num;
	char *lsb = str;
	for (int i = 0; i < size; i++){
		if (n & 1) lsb[size-i-1] = '1'; else lsb[size-i-1] = '0';
		n >>= 1;
	}
	str[size] = '\0';
}
/* 
	*(str + padding) = '\0';

	long pos;
	if (padding == 5)
		pos = 0x10;
	else if (padding == 16)
		pos = 0x8000;
	else if (padding == 26)
		pos = 0x2000000;
	else if (padding == 32)
		pos = 0x80000000;

	long mask = pos << 1;
	while (mask >>= 1)
		*str++ = !!(mask & num) + '0';
}
*/

// Convert a binary string to a decimal value
int getDec(char *bin) {

	int  b, k, m, n;
	int  len, sum = 0;

	// Length - 1 to accomodate for null terminator
	len = strlen(bin) - 1;

	// Iterate the string
	for(k = 0; k <= len; k++) {

		// Convert char to numeric value
		n = (bin[k] - '0');

		// Check the character is binary
		if ((n > 1) || (n < 0))  {
			return 0;
		}

		for(b = 1, m = len; m > k; m--)
			b *= 2;

		// sum it up
		sum = sum + n * b;
	}

	return sum;
}

