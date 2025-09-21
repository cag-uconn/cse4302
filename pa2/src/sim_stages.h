/**
 * University of Connecticut
 * CSE 4302 / CSE 5302 / ECE 5402: Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 2: Pipelined 4-Stage Simulator
 * 
 * riscy-uconn: sim_stages.h
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
struct State writeback();

void advance_pc(int step);