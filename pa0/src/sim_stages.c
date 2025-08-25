/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2025
 * Hanan Khan (Reference Implementation)
 * 
 * Programming Assignment 0
 * 
 * riscv-uconn: sim_stages.c
 * 
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "instruction_map.h"
#include "sim_core.h"
#include "sim_stages.h"
#include "decode_fields.h"

/**
 * Debug flags
 */
int debug = 0;          // Set to 1 for additional debugging information.
int pipe_trace = 1;     // Set to 1 for pipe trace.
int pipe_trace_mode = 3;


/**
 * Fetch stage implementation.
 */
struct State fetch() {

    fetch_out_n.inst = memory[pc / 4];
    fetch_out_n.inst_addr = pc;
    advance_pc(fetch_out_n.inst_addr + 4);

    //Return the instruction
    return fetch_out_n;
}

/**
 * Decode stage implementation
 */
struct State decode() {

    // read the fetch_out state and start processing decode functionality 
    decode_out_n = fetch_out;
    decode_fields(&decode_out_n);
    decode_out_n.alu_in1 = registers[decode_out_n.rs1];
    decode_out_n.alu_in2 = decode_out_n.imm;
    
    
    return decode_out_n; 
}

/**
 * Execute stage implementation
 */
struct State execute() {
    // read the decode_out state and start processing execute stage's functionality 
    ex_out_n = decode_out;
    ex_out_n.alu_out = ex_out_n.alu_in1 + ex_out_n.alu_in2;
    return ex_out_n;
}


/**
 * Writeback stage implementation
 */
unsigned int writeback() {

    // read the ex_out state and start processing writeback stage's functionality 
    wb_out_n = ex_out;
    registers[wb_out_n.rd] = wb_out_n.alu_out;
    return wb_out_n.inst;
}

/**
 * Advance PC.
 * DO NOT MODIFY.
 */

void advance_pc(int step) {
    pc_n = step;
}