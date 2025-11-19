/**
 * University of Connecticut
 * CSE 4302 / CSE 5302 / ECE 5402: Computer Architecture
 * Fall 2025
 * 
 * 
 * DO NOT MODIFY THIS FILE
 * 
 */

#pragma once

#include "sim_core.h"

extern int debug;
extern int pipe_trace;
extern int pipe_trace_mode;

void fetch();
void decode();
struct State execute();
struct State load_stage();
struct State store_stage();
void writeback();

int dcache_lookup_2_way(unsigned int addr_mem);
void dcache_update_2_way(unsigned int addr_mem, int line);


/* Related to simulator loop */
void advance_pc(int step);