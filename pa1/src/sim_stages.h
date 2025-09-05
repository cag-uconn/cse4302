/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 1: NonPipelined Simulator
 * 
 * riscv-uconn: sim_stages.h
 * 
 * DO NOT MODIFY THIS FILE  
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