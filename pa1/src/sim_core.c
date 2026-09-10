/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2026
 *
 * Programming Assignment 1: NonPipelined Simulator + Data Cache
 *
 * riscv-uconn: sim_core.c
 *
 * Core: pipeline, register file, and private data cache.
 * Data misses leave this module through mem_req and come back on mem_resp.
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
#include "sim_mem_if.h"
#include "util.h"

/**
 * Initial CPU state
 */
int registers[MAX_LENGTH] = {0};    // Registers
unsigned int pc = 0;                // Program Counter (PC) register
unsigned int pc_n = 0;              // Next Program Counter

/* Set the nop variable */
const struct State nop = {.inst = 0x0000000013, .valid = 0};

/* Initialize pipeline state */
struct State fetch_out,  fetch_out_n;
struct State decode_out, decode_out_n;
struct State ex_out,     ex_out_n;
struct State ex_ld_st_out, ex_ld_st_out_n;
struct State wb_out,    wb_out_n;

/* Cache hit is 1 cycle (LOOKUP). Miss latency lives in the memory module. */
const int dcache_access_cycles = 1;
int dmem_busy = 0;
int dmem_cycles = 0;

/* Fixed capacity / line size (see CACHE_* in sim_core.h).
 * Associativity comes from argv. */
const int cache_size      = CACHE_SIZE;
const int cache_line_size = CACHE_LINE_SIZE;
int cache_assoc           = 1;   /* set from command line: 1, 2, or 4 */

CacheSet *dcache;
int dmem_accesses = 0;
int dcache_hits = 0;

int instruction_counter = 0;
int cycle = 0;

void core_cycle(void) {
    /* Default: drop the bus request. execute_ld_st() holds it if still waiting. */
    memset(&mem_req_n, 0, sizeof(mem_req_n));

    wb_out_n       = writeback();
    ex_out_n       = execute();
    ex_ld_st_out_n = execute_ld_st();
    decode_out_n   = decode();
    fetch_out_n    = fetch();
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
    ex_ld_st_out    = nop;
    wb_out          = nop;
    wb_out_n        = nop;

    /* Zero initialize registers */
    memset(registers, 0, sizeof(registers));
    printf("Initialized Registers\n");

    printf("Initialized Memory\n");
    puts("");

    /* Allocate data cache from instructor size/line + CLI associativity */
    if (cache_line_size <= 0 || cache_assoc <= 0 || cache_assoc > CACHE_MAX_WAYS ||
        (cache_size % (cache_line_size * cache_assoc)) != 0) {
        fprintf(stderr, "[ERROR] invalid cache configuration. Aborting.\n");
        exit(1);
    }
    dcache = (CacheSet*) calloc(cache_size / (cache_line_size * cache_assoc), sizeof(CacheSet));
    if (dcache == NULL) {
        fprintf(stderr, "[ERROR] not enough memory for data cache. Aborting.\n");
        exit(1);
    }
    printf("Initialized Data Cache\n");
    puts("");

    printf("----------------------\n");
    printf("--- Section: .text ---\n");
    printf("----------------------\n");

    char line[MAX_LENGTH + 2];
    char *p;
    int i = 0, line_num = 0;

    /* Copy .text section to memory, break at nop */
    while (fgets(line, MAX_LENGTH + 2, fp) != NULL) {
        line_num++;

        p = strchr(line, '\n');
        if (p != NULL) {
            *p = '\0';
        }

        memory[i] = getDec(line);

        if (strcmp(line, "11111111111111111111111111111111") == 0) {
            memory[i] = 0;
            i = 256;
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

    int bytes = 33 * line_num;
    fseek(fp, bytes, SEEK_SET);

    while (fgets(line, MAX_LENGTH + 2, fp) != NULL) {
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
