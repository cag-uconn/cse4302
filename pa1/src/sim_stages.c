/**
 * University of Connecticut
 * Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 1: 4-stage Unpipelined Simulator
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

    /*Insert your code here*/

    //Return the instruction
    return fetch_out_n;
}

/**
 * Decode stage implementation
 */
struct State decode() {

    // read the fetch_out state and start processing decode functionality 
    decode_out_n = fetch_out;
    
    /*Insert your code here*/
    
    return decode_out_n; 
}

/**
 * Execute stage implementation
 */
struct State execute() {
    // read the decode_out state and start processing execute stage's functionality 
    ex_out_n = decode_out;
    
    /*Insert your code here*/

    return ex_out_n;
}


/**
 * Writeback stage implementation
 */
struct State writeback() {

    // read the ex_out state and start processing writeback stage's functionality 
    wb_out_n = ex_out;
    
    /*Insert your code here*/

    return wb_out_n.inst;
}


/**
 * Advance PC.
 * DO NOT MODIFY.
 */

void advance_pc(int step) {
    pc_n = step;
}