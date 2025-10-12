/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 3: MultiCycle Stall + Out Of Order + Seperate Execution Units
 * 
 * riscy-uconn: sim_stages.c
 * 
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction_map.h"
#include "sim_core.h"
#include "sim_stages.h"
#include "decode_fields.h"

/**
 * Debug flags
 */
int debug = 0;           /* Set to 1 for additional debugging information. */
int pipe_trace = 1;      /* Set to 1 for pipe trace. */
int pipe_trace_mode = 3; 



/**
 * Fetch stage implementation.
 */
struct State fetch() {

    /*Insert your code here*/
    
}

/**
 * Decode stage implementation
 */
struct State decode() {
    
    /*Insert your code here*/
}

/**
 * Execute stage implementation
 */
struct State execute() {

    /*Insert your code here*/
}



/**
 * Implementation for Loads
 */
struct State load_stage() {


    /*Insert your code here*/

   
}


/**
 * Implementation for Stores
 */
struct State store_stage() {
    
    /*Insert your code here*/
    
}

/**
 * Writeback stage implementation
 */
void writeback() {

    /*Insert your code here*/
    
    
    
}

/**
 * Advance PC.
 * DO NOT MODIFY.
 */

void advance_pc(int step) {
    pc_n = step;
}
