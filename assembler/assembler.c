/*
 * assembler.c
 *
 *  Created on: Oct 3, 2011
 *      Modified by Masab Ahmad
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_parser.h"
#include "hash_table.h"

int search(char *instruction);

// Array that holds the supported instructions
char *instructions[] = {
		"add",	// 0
		"sub",	// 1
		"slt",   // 3
		"sll",	// 4
		"srl",	// 5
		"and",	// 6
		"or",	// 7
		"xor",	// 8
		"jalr",	// 9
		"addi",	// 19
		"slti",	// 11
		"andi",	// 12
		"ori",	// 12
		"xori", // 13
		"slli",	// 14
		"srli",	// 15
		"lw",	// 16
		"beq",	// 17
        "bne",  // 18
		"blt",	// 19
		"bge",	// 20
		"sw",   // 21
		"jal",  // 22
		"lui"	// 23
};
		


// Size of array
size_t inst_len = sizeof(instructions)/sizeof(char *);

int search(char *instruction) {

	int found = 0;

	for (int i = 0; i < inst_len; i++) {
		if (strcmp(instruction, instructions[i]) == 0) {
			found = 1;
			return i;
		}
	}

	if (found == 0)
		return -1;
}

// Quick Sort String Comparison Function
int string_comp(const void *a, const void *b) {
	return strcmp(*(char **)a, *(char **)b);
}

int main (int argc, char *argv[]) {

	// Make sure correct number of arguments input
	if (argc != 3) {
		printf("Incorrect number of arguments");
	}

	else {

		// Open I/O files
		// Check that files opened properly
		FILE *In;
		In = fopen(argv[1], "r");
		if (In == NULL) {
			printf("Input file could not be opened.");
			exit(1);
		}

		FILE *Out;
		Out = fopen(argv[2], "w");
		if (Out == NULL) {
			printf("Output file could not opened.");
			exit(1);
		}

		// Sort the array using qsort for faster search
		qsort(instructions, inst_len, sizeof(char *), string_comp);

		// Create a hash table of size 127
		hash_table_t *hash_table = create_hash_table(127);

		// Parse in passes

		int passNumber = 1;
		parse_file(In, passNumber, instructions, inst_len, hash_table, Out);

		// Rewind input file & start pass 2
		rewind(In);
		passNumber = 2;
		parse_file(In, passNumber, instructions, inst_len, hash_table, Out);

		// Close files
		fclose(In);

		fclose(Out);
		int size = fseek(Out, 0, SEEK_END);
		return 0;
	}
}
