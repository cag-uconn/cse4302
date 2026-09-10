/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2026
 *
 * Programming Assignment 1: NonPipelined Simulator + Data Cache
 *
 * riscv-uconn: sim_memory.h
 *
 * Shared DRAM plus a 2-state wait FSM (IDLE / BUSY).
 *
 * DO NOT MODIFY THIS FILE
 */

#pragma once

#include "sim_mem_if.h"

#define MEMORY_SIZE 16384

void memory_init(void);
void memory_free(void);

/* Clock the memory server FSM. Reads mem_req, writes mem_resp_n. */
void memory_cycle(void);

/* Combinational instruction fetch (I-port). Not the data-request bus. */
unsigned int memory_inst_fetch(unsigned int byte_pc);
