/**
 * University of Connecticut
 * CSE 4302 / CSE 5302 / ECE 5402: Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 3: MultiCycle Stall + Out Of Order + Seperate Execution Units
 * 
 * riscy-uconn: instruction_map.h
 * 
 * DO NOT MODIFY THIS FILE
 * 
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
const struct State nop = {.inst = 0x0000000013};

/* Different settings that can be enabled */
int forwarding_enabled  = 0;
int ooo_enabled  = 0;


/* Initialize pipeline state */
struct State fetch_out,  fetch_out_n;
struct State decode_out, decode_out_n;
struct State ex_out,     ex_out_n;
struct State  ld_out,  ld_out_n;
struct State st_out, st_out_n;
struct State wb_out, wb_out_n;
struct State wb_ld_out, wb_ld_out_n;


/* Pipeline-related initialization */
int pipe_stall          = 0;
int j_taken             = 0; 
int we_exe              = 0,   ws_exe         = 0, dout_exe         = 0;
int we_mem              = 0,   ws_mem         = 0, dout_mem         = 0;
int we_wb               = 0,   ws_wb          = 0, dout_wb          = 0;
int we_ld_wb            = 0,   ws_ld_wb       = 0, dout_ld_wb       = 0;

/* Multi-cycle operation-related initialization */
const int dmem_access_cycles = 6;
int dmem_busy = 0;
int dmem_cycles = 0;

/* Branch Related */
int br_mispredicted = 0;

/* Count all non-nop instructions */
int instruction_counter = 0;  
 

/* Total Cycles */
int cycle = 0;     

/* BHSR/BHT info */
char BHSR_string[5];
char BHT_string[3];

/**
 * Simulator entry point
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "[ERROR] incorrect number of arguments.\n");
        printf("usage: simulator PROGRAM_FILE FORWARDING_ENABLED OOO_ENABLED\n");
        exit(1);
    } else {
        /* Open input program file */
        FILE *fp;
        fp = fopen(argv[1], "r");

        /* Enable/disable forwarding */
        sscanf(argv[2],"%d", &forwarding_enabled);
        if (forwarding_enabled > 1) {
            fprintf(stderr, "[ERROR] FORWARDING_ENABLED must be either 0 (disabled) or 1 (enabled).\n");
            exit(1);
        }
        printf("Pipeline Forwarding: %s\n", forwarding_enabled ? "Enabled" : "Disabled");

        /* Enable/disable OOO */
        sscanf(argv[3],"%d", &ooo_enabled);
        if (ooo_enabled > 1) {
            fprintf(stderr, "[ERROR] OOO_ENABLED must be either 0 (disabled) or 1 (enabled).\n");
            exit(1);
        }
        printf("OOO Execution: %s\n", ooo_enabled ? "Enabled" : "Disabled");

        
        
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
    unsigned int committed_inst;
    unsigned int committed_inst_ld_st;
    while (terminate != 1) {
        
        /* Update pipeline state */
        
                                  writeback();
        ex_out_n                = execute();
        ld_out_n                = load_stage();
        st_out_n                = store_stage();
        decode_out_n            = decode();
        fetch_out_n             = fetch();
        

        //increment cycle counter
        cycle++;

        /* Increment the total instruction counter */
        if (wb_out_n.valid == 1){
            instruction_counter++;
        }

        if (ooo_enabled)
        {
            /* Increment the total instruction counter */
            if (wb_ld_out_n.valid == 1){
                instruction_counter++;
            }         
        }

        if (pipe_trace == 1) {
            fprintf(fptr_pt, "Cycle %d, PC %d, Next PC %d\n", cycle, pc, pc_n);
            // fprintf(fptr_pt, "dmem_busy : %d | dmem_busy2 : %d | dmem_cycles: %d | dmem_cycles2: %d \n", dmem_busy,dmem_busy2,dmem_cycles,dmem_cycles2);
            // fprintf(fptr_pt, "instr_type_1: %d | instr_type_2 : %d \n", instr_type_1,instr_type_2);
            // fprintf(fptr_pt, "instruction_counter : %d \n", instruction_counter);
            // fprintf(fptr_pt, "pipe_stall : %d \n", pipe_stall);
            // fprintf(fptr_pt, "we_mem : %d | ws_mem: %d \n", we_mem,ws_mem);
            inst_dump("[Fetch]",     fetch_out_n.inst);
            inst_dump("[Decode]",    decode_out_n.inst);
            inst_dump_parallel("[Execute ALU]",   ex_out_n.inst);
            
            inst_dump_parallel("   [Execute LD]",    ld_out_n.inst);
            inst_dump("   [Execute ST]",   st_out_n.inst);

            if (ooo_enabled) {
                inst_dump_parallel("[Writeback ALU or ST]",    wb_out_n.inst);
                inst_dump_parallel("[Writeback LD]", wb_ld_out_n.inst);
                // inst_dump("     [Writeback ST]", wb_ld_st_2_out_n.inst);
            }
            else {
                inst_dump_parallel("[Writeback]",    wb_out_n.inst);

            }    
            

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
            fprintf(stderr, "[DEBUG] Cycle: %d, Committed Instruction: 0x%08x\n", cycle, committed_inst);
        }

        if (registers[0] != 0) {
            terminate = 1;  // set terminate flag when $zero is updated
        }

        /* Update state for next cycle */
        pc              = pc_n;
        decode_out      = decode_out_n;
        fetch_out       = fetch_out_n;
        ex_out          = ex_out_n;
        ld_out          =  ld_out_n;
        st_out          = st_out_n;
        wb_out          = wb_out_n;
        wb_ld_out       = wb_ld_out_n;
        
        
        if (cycle == 25000) {
            fprintf(stderr, "\n[WARNING] Simulation has simulated 25,000 cycles without terminating. Something might be wrong. Terminating.\n");
            exit(1);
        }
    }

    
    printf("\nFinished simulation!\n");
    printf("\nTOTAL INSTRUCTIONS COMMITTED: %d\n", instruction_counter);
    printf("TOTAL CYCLES SIMULATED: %d\n", cycle);
    printf("AVERAGE CPI: %0.3f\n\n", (double)cycle/(double)instruction_counter);

   
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
    fetch_out       = nop;
    decode_out      = nop;
    ex_out          = nop;
    ld_out          = nop;
    st_out          = nop;
    wb_out          = nop;
    wb_ld_out       = nop;
    
    fetch_out_n       = nop;
    decode_out_n      = nop;
    ex_out_n          = nop;
    ld_out_n          = nop;
    st_out_n          = nop;
    wb_out_n          = nop;
    wb_ld_out_n       = nop;
    
    /* Zero initialize registers */
    memset(registers, 0, sizeof(registers));
    printf("Initialized Registers\n");
    puts("");

    /* Allocate instruction and data memory */
    memory = (int*) malloc(16384 * sizeof(int));
    if (memory == NULL) {
        fprintf(stderr, "[ERROR] not enough memory for data memory. Aborting.\n");
        exit(1);
    }

    /* Initialize instruction memory to "nop" -1 */
    for (int i = 0; i < 256; i++) {
        memory[i] = nop.inst;
    }

    /* Initialize data memory to "nop" -1 */
    for (int i = 256; i < 16384; i++) {
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
            memory[i] = nop.inst;
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
