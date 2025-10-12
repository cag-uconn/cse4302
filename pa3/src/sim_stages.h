/**
 * University of Connecticut
 * CSE 4302 / CSE 5302 / ECE 5402: Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 3: MultiCycle Stall + In order + Seperate Execution Units
 * 
 * riscy-uconn: instruction_map.h
 * 
 * DO NOT MODIFY THIS FILE
 * 
 */

#pragma once

#include "sim_core.h"

extern int debug;
extern int pipe_trace;
extern int pipe_trace_mode;

struct State fetch();
struct State decode();
struct State execute();
struct State load_stage();
struct State store_stage();
void writeback();


/* Related to simulator loop */
void advance_pc(int step);