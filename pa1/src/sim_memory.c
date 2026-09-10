/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2026
 *
 * Programming Assignment 1: NonPipelined Simulator + Data Cache
 *
 * riscv-uconn: sim_memory.c
 *
 * Memory-side state machine (2 states only):
 *   MEM_IDLE  : wait for a rising edge on mem_req.valid
 *   MEM_BUSY  : count Miss_latency cycles, raise ack, return to IDLE
 *
 * The core LOOKUP cycle (miss detect + assert valid) already counts as
 * miss cycle 1. On accept we therefore start mem_cycles at 2, and ack
 * when ++mem_cycles reaches Miss_latency. With Miss_latency == 6 a
 * load/store is: hit = 4 cycles total, miss = 9 cycles total.
 *
 * DO NOT MODIFY THIS FILE
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sim_memory.h"

const int Miss_latency = 6;

int *memory = NULL;

typedef enum {
    MEM_IDLE = 0,
    MEM_BUSY
} MemServerState;

static MemServerState mem_state = MEM_IDLE;
static int mem_cycles = 0;
static int prev_valid = 0;   /* for rising-edge detect on mem_req.valid */

void memory_init(void) {
    memory = (int*) malloc(MEMORY_SIZE * sizeof(int));
    if (memory == NULL) {
        fprintf(stderr, "[ERROR] not enough memory for DRAM. Aborting.\n");
        exit(1);
    }
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = -1;
    }
    mem_state = MEM_IDLE;
    mem_cycles = 0;
    prev_valid = 0;
    memset(&mem_resp, 0, sizeof(mem_resp));
    memset(&mem_resp_n, 0, sizeof(mem_resp_n));
}

void memory_free(void) {
    free(memory);
    memory = NULL;
}

unsigned int memory_inst_fetch(unsigned int byte_pc) {
    return (unsigned int) memory[byte_pc / 4];
}

void memory_cycle(void) {
    mem_resp_n.ack = 0;

    switch (mem_state) {
        case MEM_IDLE:
            /* Start only on a new request (0 -> 1), so holding valid
             * across the ack cycle does not restart the timer. */
            if (mem_req.valid && !prev_valid) {
                /* LOOKUP already used miss cycle 1; continue from 2. */
                mem_cycles = 2;
                mem_state = MEM_BUSY;
            }
            break;

        case MEM_BUSY:
            if (++mem_cycles == Miss_latency) {
                mem_resp_n.ack = 1;
                mem_cycles = 0;
                mem_state = MEM_IDLE;
            }
            break;
    }

    prev_valid = mem_req.valid;
}
