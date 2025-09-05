/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 1: NonPipelined Simulator
 * 
 * riscv-uconn: sim_core.h
 * 
 * DO NOT MODIFY THIS FILE  
 */

#pragma once

#include <stdio.h>

extern FILE *fptr_pt;

/* Max number of registers, and instruction length in bits */
#define MAX_LENGTH 32

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
extern struct State wb_out, wb_out_n;

/* nop instruction, used when flushing the pipeline */
extern const struct State nop;

/* Instruction and data memory */
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




void initialize(FILE *fp);
void process_instructions();