/**
 * University of Connecticut
 * Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 1: 4-stage unpipelined Simulator
 * 
 * riscv-uconn: sim_core.c
 * 
 * DO NOT MODIFY THIS FILE  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction_map.h"
#include "register_map.h"
#include "sim_core.h"
#include "sim_stages.h"
#include "util.h"

/**
 * Initial CPU state
 */
int registers[MAX_LENGTH] = {0};    // Registers
unsigned int pc = 0;                // Program Counter (PC) register
unsigned int pc_n = 0;              // Next Program Counter
int *memory = NULL;                 // Data & instruction memory

/**
 * Utility
 */
FILE *fptr_pt;


/* Set the nop variable */
const struct State nop = {.inst = 0x0000000013, .valid = 0};

/* Initialize pipeline state */
struct State fetch_out,  fetch_out_n;
struct State decode_out, decode_out_n;
struct State ex_out,     ex_out_n;
struct State wb_out,    wb_out_n;



/* Count all non-nop instructions */
int instruction_counter = 0;  

/* Total Cycles */
int cycle = 0;                     

/**
 * Simulator entry point
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "[ERROR] incorrect number of arguments.\n");
        printf("usage: simulator PROGRAM_FILE\n");
        exit(1);
    } else {
        /* Open input program file */
        FILE *fp;
        fp = fopen(argv[1], "r");

       
        /* Open pipe trace */
        if (pipe_trace) {
            fptr_pt = fopen("pipe_trace.txt", "w");
        }

        /* Initialize registers and instruction/data memory */
        initialize(fp);

        puts("\n");
        printf("Simulating...\n");

        /* Process instructions one at a time */
        process_instructions();

        puts("");

        /* Output state after termination */
        rdump();    // Register dump
        mdump_modified();
        mdump();    // Memory dump

        /* Cleanup */
        free(memory);
        fclose(fp);
        if (pipe_trace) {
            fclose(fptr_pt);
        }

        return 0;
    }
}

void process_instructions() {
    int terminate = 0;
    while (terminate != 1) {
        
        /* Update pipeline state */
        wb_out_n                = writeback();
        ex_out_n                = execute();
        decode_out_n            = decode();
        fetch_out_n             = fetch();

        /* Increment the total instruction counter */
        if (wb_out_n.valid == 1){
            instruction_counter++;
        }
        /* Increment the cycle counter */
        cycle++;

        if (pipe_trace == 1) {
            fprintf(fptr_pt, "Cycle %d, PC %d, Next PC %d\n", cycle, pc, pc_n);
            // fprintf(fptr_pt, "instruction_counter : %d \n", instruction_counter);
            inst_dump("[Fetch]",     fetch_out_n.inst);
            inst_dump("[Decode]",    decode_out_n.inst);
            inst_dump("[Execute]",   ex_out_n.inst);
            inst_dump("[Writeback]", wb_out_n.inst);
            fprintf(fptr_pt, "\n");
            rdump_pt();
            fprintf(fptr_pt, "\n");
            fprintf(fptr_pt, "=================================================================================================================================\n");
            fprintf(fptr_pt, "\n");
        }

        if (debug) {
            fprintf(stderr, "[DEBUG] Cycle: %d, Instruction Memory Address: %d, Instruction: 0x%08x\n", cycle, pc / 4, fetch_out_n.inst);
        }

        if (debug) {
            fprintf(stderr, "[DEBUG] Cycle: %d, Committed Instruction: 0x%08x\n", cycle, instruction_counter);
        }

        if (registers[0] != 0) {
            terminate = 1;  // set terminate flag when $zero is updated
        }

        /* Update state for next cycle */
        pc          = pc_n;
        fetch_out   = fetch_out_n; 
        decode_out  = decode_out_n;
        ex_out      = ex_out_n;
        wb_out      = wb_out_n;

        if (cycle == 25000) {
            fprintf(stderr, "\n[WARNING] Simulation has simulated 25,000 cycles without terminating. Something might be wrong. Terminating.\n");
            exit(1);
        }
    }
    printf("\nFinished simulation!\n");
    printf("\nTOTAL INSTRUCTIONS COMMITTED: %d\n", instruction_counter);
    printf("TOTAL CYCLES SIMULATED: %d\n", cycle);
    printf("AVERAGE CPI: %0.3f\n", (double)cycle/(double)instruction_counter);
}

void initialize(FILE *fp) {
    printf("======================================\n");
    printf("=== BEGIN SIMULATOR INITIALIZATION ===\n");
    printf("======================================\n");
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] opening input file. Aborting.\n");
        exit(1);
    }

    /* Initialize the starting states */
    fetch_out  = nop;
    decode_out = nop;
    ex_out     = nop;
    wb_out     = nop;
    wb_out_n   = nop;
    
    /* Zero initialize registers */
    memset(registers, 0, sizeof(registers));
    printf("Initialized Registers\n");

    /* Allocate instruction and data memory */
    memory = (int*) malloc(16384 * sizeof(int));
    if (memory == NULL) {
        fprintf(stderr, "[ERROR] not enough memory. Aborting.\n");
        exit(1);
    }

    /* Initialize memory to -1 */
    for (int i = 0; i < 16384; i++) {
        memory[i] = -1;
    }
    printf("Initialized Memory\n");
    puts("");

    printf("----------------------\n");
    printf("--- Section: .text ---\n");
    printf("----------------------\n");

    /* Initialize parsing variables */
    char line[MAX_LENGTH + 2];
    char *p;
    int i = 0, line_num = 0;

    /* Copy .text section to memory, break at nop */
    while (fgets(line, MAX_LENGTH + 2, fp) != NULL) {
        line_num++;

        /* Remove '\n' from 'line' */
        p = strchr(line, '\n');
        if (p != NULL) {
            *p = '\0';
        }

        memory[i] = getDec(line);

        /* If 'nop' found, move to 0x8000 / 2048 in memory and break */
        if (strcmp(line, "11111111111111111111111111111111") == 0) {
            memory[i] = 0;
            i = 256; //Data Memory Starts at 256
            break;
        } else {
            printf("memory[%d] = 0x%08x\n", i, memory[i]);
            i++;
        }
    }
    int j;              
    for (j = i; j < 16384; j++) {
        memory[j] = 0;
    }


    puts("");

    printf("----------------------\n");
    printf("--- Section: .data ---\n");
    printf("----------------------\n");

    /* Seek fp to first instruction in .data */
    char data[MAX_LENGTH + 2];
    int bytes = 33 * line_num;
    fseek(fp, bytes, SEEK_SET);

    /* Copy .data section to memory */
    while (fgets(line, MAX_LENGTH + 2, fp) != NULL) {
        /* Remove '\n' from 'line' */
        p = strchr(line, '\n');
        if (p != NULL) {
            *p = '\0';
        }

        memory[i] = getDec(line);
        printf("memory[%d] = 0x%08x\n", i, memory[i]);
        i++;
    }

    printf("====================================\n");
    printf("=== END SIMULATOR INITIALIZATION ===\n");
    printf("====================================");
}