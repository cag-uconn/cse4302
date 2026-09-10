/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2026
 *
 * Programming Assignment 1: NonPipelined Simulator + Data Cache
 *
 * riscv-uconn: sim_mem_if.h
 *
 * Simple request / ack handshake between core(s) and memory.
 *
 *   CORE  ---- mem_req.valid  ---->  MEMORY
 *   CORE  <--- mem_resp.ack   ----   MEMORY
 *
 *
 * One core for now. Later, an arbiter in sim_top.c can mux several
 * core request ports onto this same memory port.
 *
 * DO NOT MODIFY THIS FILE
 */

#pragma once

/* Core -> Memory: "I need a DRAM access" (held until ack) */
typedef struct {
    int valid;           /* 1 = request outstanding */
} MemReq;

/* Memory -> Core: "latency has elapsed" */
typedef struct {
    int ack;             /* 1 = request complete this cycle */
} MemResp;

/* Registered bus wires (latched in sim_top.c) */
extern MemReq  mem_req,  mem_req_n;
extern MemResp mem_resp, mem_resp_n;
