/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2026
 *
 * Programming Assignment 1: NonPipelined Simulator + Data Cache
 *
 * riscv-uconn: sim_top.c
 *
 * Top level: one core and one memory, tied together with the
 * request / ack protocol in sim_mem_if.h.
 *
 *        +--------+     mem_req      +--------+
 *        |  CORE  | ---------------> | MEMORY |
 *        | (cache | <--------------- | (DRAM  |
 *        |  inside)|    mem_resp     |  + FSM)|
 *        +--------+                  +--------+
 *
 * Later, extra cores attach here. An arbiter would sit on the
 * request path; the memory FSM does not change.
 *
 * DO NOT MODIFY THIS FILE
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction_map.h"
#include "register_map.h"
#include "sim_core.h"
#include "sim_stages.h"
#include "sim_memory.h"
#include "sim_mem_if.h"
#include "util.h"

MemReq  mem_req  = {0}, mem_req_n  = {0};
MemResp mem_resp = {0}, mem_resp_n = {0};

FILE *fptr_pt;
FILE *fptr_mt;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "[ERROR] incorrect number of arguments.\n");
        printf("usage: simulator PROGRAM_FILE ASSOCIATIVITY\n");
        printf("  ASSOCIATIVITY: 1 = direct mapped, 2 = 2-way, 4 = 4-way\n");
        exit(1);
    }

    cache_assoc = atoi(argv[2]);
    if (cache_assoc != 1 && cache_assoc != 2 && cache_assoc != 4) {
        fprintf(stderr, "[ERROR] ASSOCIATIVITY must be 1, 2, or 4.\n");
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");

    if (pipe_trace) {
        fptr_pt = fopen("pipe_trace.txt", "w");
    }
    if (cache_trace) {
        fptr_mt = fopen("cache_trace.txt", "w");
    }

    memory_init();
    initialize(fp);

    printf("\nData Cache Associativity = %d\n", cache_assoc);
    puts("\n");
    printf("Simulating...\n");

    process_instructions();

    puts("");

    rdump();
    mdump_modified();
    mdump();
    cdump();

    memory_free();
    free(dcache);
    fclose(fp);
    if (pipe_trace) {
        fclose(fptr_pt);
    }
    if (cache_trace) {
        fclose(fptr_mt);
    }

    return 0;
}

void process_instructions() {
    int terminate = 0;
    while (terminate != 1) {

        /*
         * Clock both sides of the bus, then the core pipeline.
         * Memory uses the registered request (mem_req) and produces
         * mem_resp_n. The core uses the registered ack (mem_resp)
         * and produces mem_req_n.
         */
        memory_cycle();
        core_cycle();

        /* Increment the total instruction counter */
        if (wb_out_n.valid == 1) {
            instruction_counter++;
        }
        /* Increment the cycle counter */
        cycle++;

        if (pipe_trace == 1) {
            // if (ex_ld_st_out_n.valid == 1) {
            //     if (ex_ld_st_out_n.opcode == ITYPE_LOAD) {
            //         fprintf(fptr_pt, "Cycle %d, LD addr %d, dcache_hit %d\n", cycle, ex_ld_st_out_n.mem_addr, ex_ld_st_out_n.dcache_hit);
            //     } else if (ex_ld_st_out_n.opcode == STYPE) {
            //         fprintf(fptr_pt, "Cycle %d, ST addr %d, dcache_hit %d\n", cycle, ex_ld_st_out_n.mem_addr, ex_ld_st_out_n.dcache_hit);
            //     }
            // }
            // fprintf(fptr_pt, "mem_req.valid %d ack %d dmem_busy %d\n",
            //         mem_req.valid, mem_resp.ack, dmem_busy);
            inst_dump("[Fetch]",          fetch_out_n.inst);
            inst_dump("[Decode]",         decode_out_n.inst);
            inst_dump("[Execute ALU]",    ex_out_n.inst);
            inst_dump("[Execute LD ST]",  ex_ld_st_out_n.inst);
            inst_dump("[Writeback]",      wb_out_n.inst);
            fprintf(fptr_pt, "\n");
            rdump_pt();
            fprintf(fptr_pt, "\n");
            fprintf(fptr_pt, "=================================================================================================================================\n");
            fprintf(fptr_pt, "\n");
        }

        if (debug) {
            fprintf(stderr, "[DEBUG] Cycle: %d, Instruction Memory Address: %d, Instruction: 0x%08x\n", cycle, pc / 4, fetch_out_n.inst);
        }

        if (debug) {
            fprintf(stderr, "[DEBUG] Cycle: %d, Committed Instruction: 0x%08x\n", cycle, instruction_counter);
        }

        if (registers[0] != 0) {
            terminate = 1;
        }

        /* Latch pipeline and bus for the next cycle */
        pc           = pc_n;
        fetch_out    = fetch_out_n;
        decode_out   = decode_out_n;
        ex_out       = ex_out_n;
        ex_ld_st_out = ex_ld_st_out_n;
        wb_out       = wb_out_n;
        mem_req      = mem_req_n;
        mem_resp     = mem_resp_n;

        if (cycle == 250000) {
            fprintf(stderr, "\n[WARNING] Simulation has simulated 250,000 cycles without terminating. Something might be wrong. Terminating.\n");
            exit(1);
        }
    }

    float dcache_hitrate = 0;
    if (dmem_accesses) {
        dcache_hitrate = ((float)dcache_hits / (float)dmem_accesses) * 100;
    }

    printf("\nFinished simulation!\n");
    printf("\nTOTAL INSTRUCTIONS COMMITTED: %d\n", instruction_counter);
    printf("TOTAL CYCLES SIMULATED: %d\n", cycle);
    printf("AVERAGE CPI: %0.3f\n\n", (double)cycle / (double)instruction_counter);

    printf("TOTAL MEMORY ACCESSES: %d\n", dmem_accesses);
    printf("CACHE HITS: %d\n", dcache_hits);
    printf("CACHE HIT RATE: %.2f%%\n", dcache_hitrate);
}
