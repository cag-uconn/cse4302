/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2026
 * 
 * Programming Assignment 1: NonPipelined Simulator + Data Cache
 * 
 * riscv-uconn: sim_core.h
 * 
 * DO NOT MODIFY THIS FILE  
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>

extern FILE *fptr_pt;
extern FILE *fptr_mt;

/* Max number of registers, and instruction length in bits */
#define MAX_LENGTH 32

/* Fixed capacity and line size. Max ways = fully associative */
#define CACHE_SIZE      256
#define CACHE_LINE_SIZE 16
#define CACHE_MAX_WAYS  (CACHE_SIZE / CACHE_LINE_SIZE)

/* Array of registers (register file) */
extern int registers[MAX_LENGTH];

/* Clock cycle */
extern int cycle;

/* Program Counter (PC) register */
extern unsigned int pc;     // Current PC
extern unsigned int pc_n;   // Next PC

/* Microarchitechtual state */
extern struct State fetch_out, fetch_out_n;
extern struct State decode_out, decode_out_n;
extern struct State ex_out, ex_out_n;
extern struct State ex_ld_st_out, ex_ld_st_out_n;
extern struct State wb_out, wb_out_n;

/* nop instruction, used when flushing the pipeline */
extern const struct State nop;

/* Instruction and data memory (owned by the memory module) */
extern int *memory;

/* Instruction and cycle counters */
extern int instruction_counter;
extern int cycle;

/* CPU state */
struct State {
     /* Fetched instruction */
     unsigned int inst;
     unsigned int inst_addr;

     /* Decoded instruction fields */
     unsigned int opcode;
     unsigned int funct3;
     unsigned int funct7;
     unsigned int rd;
     unsigned int rs1;
     unsigned int rs2;
     unsigned int imm;

     /* Memory related */
     unsigned int mem_buffer;
     unsigned int mem_addr;

     /* Branch Related */
     unsigned int br_addr;
     unsigned int link_addr;

     /* ALU */
     unsigned int alu_in1;
     unsigned int alu_in2;
     unsigned int alu_out;

     unsigned int valid;

     unsigned int br_taken;
};

/* Multi-cycle memory / cache */
extern const int Miss_latency;
extern const int dcache_access_cycles;
extern int dmem_busy;
extern int dmem_cycles;

/* Data Cache-related */
extern int dmem_accesses;
extern int dcache_hits;

/* Cache geometry: size / line size from CACHE_* macros above.
 * Associativity is set from the command line in sim_top.c before initialize(). */
extern const int cache_size;
extern const int cache_line_size;
extern int cache_assoc;

/* Structure that defines the cache block */
typedef struct {
     unsigned int tag;
     unsigned int valid;
} CacheBlock;

/* One cache set. block[] has room for CACHE_MAX_WAYS ways;
 * only cache_assoc of them are used.
 * lru1 is the replacement-state bit. */
typedef struct {
     CacheBlock block[CACHE_MAX_WAYS];
     bool lru1;
     bool lru2;
     bool lru3;
} CacheSet;

extern CacheSet *dcache;

void initialize(FILE *fp);
void process_instructions();
void core_cycle(void);
