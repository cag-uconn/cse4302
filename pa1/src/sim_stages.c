/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2026
 * Afif Siddiqi (Reference Implementation)
 * 
 * Programming Assignment 1: NonPipelined Simulator + Data Cache
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
#include "sim_cache.h"
#include "sim_mem_if.h"
#include "sim_memory.h"
#include "decode_fields.h"

/**
 * Debug flags
 */
int debug = 0;          // Set to 1 for additional debugging information.
int pipe_trace = 1;     // Set to 1 for pipe trace.
int pipe_trace_mode = 3;
int cache_trace = 1;

/**
 * Fetch stage implementation.
 */
struct State fetch() {
    
    
    //check if there is a valid instruction in decode, either execute unit, or wb
    //if yes then fetch then return a nop and set valid to 0
    if (decode_out_n.valid == 1 || ex_out_n.valid == 1 || ex_ld_st_out_n.valid == 1 || wb_out_n.valid == 1 || dmem_busy){
        fetch_out_n.valid = 0;
        return nop;
    }else{
        fetch_out_n.valid = 1;
    }

    
    fetch_out_n.inst = memory_inst_fetch(pc);
    fetch_out_n.inst_addr = pc;
    
    
    //Return the instruction
    return fetch_out_n;
}

/**
 * Decode stage implementation
 */
struct State decode() {

    // read the fetch_out state and start processing decode functionality 
    decode_out_n = fetch_out;
   
    // struct State *ptr = &fetch_out;
    decode_fields(&decode_out_n);
    

    /* Used to check if rs1 pr rs2 is read */
    int rs1_read = 0;
    int rs2_read = 0;

    /**
     * Determine which registers the current instruction will read
     */
    switch(decode_out_n.opcode){
        /* All R-types, B-types, and S-types use rs1 / rs2 as operands */
        case RTYPE:
        case BTYPE:
        case STYPE:
            rs1_read = 1; 
            rs2_read = 1;
            break;

        /* All I-types only read from rs1 */
        case ITYPE_ARITH:
        case ITYPE_LOAD:
        case JALR:
            rs1_read = 1;
        
        /* Other instructions don't do reads on rs1 / rs2*/
        default:
            break;
    }

    /**
     * Register source values. They initialize to the register file, 
     * but will be set to the most up-to-date values.
    */
    int rs1_source = registers[decode_out_n.rs1];
    int rs2_source = registers[decode_out_n.rs2];
    int raw_hazard = 0;
    
   
    /**
     * Instruction-specific decode logic (should now read rs1/rs2_source rather than directly from register)
     */
    switch(decode_out_n.opcode){

        /* All R-types use rs1 / rs2 as alu1 / alu2 */
        case RTYPE:
            decode_out_n.alu_in1 = rs1_source;
            decode_out_n.alu_in2 = rs2_source;
            break;

        /* jalr , I-type arithmatic, and load all use upper 12 bits for imm, rs1 for alu1, and imm for alu2 */
        case JALR:
        case ITYPE_ARITH:
        case ITYPE_LOAD:
            decode_out_n.alu_in1 = rs1_source;
            decode_out_n.alu_in2 = decode_out_n.imm;
            break;
        
        /* S-type needs to decode the immediate accordingly, use rs1 as alu1, imm as alu2, rs2 as word-to-be-saved */
        case STYPE:
            decode_out_n.alu_in1 = rs1_source;
            decode_out_n.alu_in2 = decode_out_n.imm;
            decode_out_n.mem_buffer = rs2_source; //SW needs to save the read register for MEM write
            break;

        /* B-type need to decode imm accordingly, use rs1 / rs2 as alu1 / alu2, calculate branch address if branch taken */
        case BTYPE:
            decode_out_n.alu_in1 = rs1_source;
            decode_out_n.alu_in2 = rs2_source;
            break;

        /* lui does nothing */
        case LUI:
            break;

        /* jal needs to decode imm accordingly, link the address, and change the next pc */
        case JAL:
            break;
    }

    
    return decode_out_n; 
}

/**
 * Execute stage implementation
 */
struct State execute() {

    // read the decode_out state and start processing execute stage's functionality 
    ex_out_n = decode_out;

    // ALU execute unit does not handle loads or stores
    if (ex_out_n.valid == 0 || ex_out_n.opcode == ITYPE_LOAD || ex_out_n.opcode == STYPE) {
        return nop;
    }

    int take_branch;

    
    /* Start decoding instructions */
    switch(ex_out_n.opcode){   
        /* Check if it is R-type or arithmatic I-type */
        case RTYPE:
        case ITYPE_ARITH:
            switch(ex_out_n.funct3){
                /* add, addi, or sub */ 
                case ADD_SUB:
                    /* sub */
                    if (ex_out_n.opcode == RTYPE && ex_out_n.funct3 == ADD_SUB && ex_out_n.funct7 == SUB_F7){
                        ex_out_n.alu_out = ex_out_n.alu_in1 - ex_out_n.alu_in2;
                    
                    /* add, addi */
                    }else{ 
                        ex_out_n.alu_out = ex_out_n.alu_in1 + ex_out_n.alu_in2;
                    }
                    break; 

                /* and, andi */
                case AND:
                    ex_out_n.alu_out = ex_out_n.alu_in1 & ex_out_n.alu_in2;
                    break;

                /* or, ori */
                case OR:
                    ex_out_n.alu_out = ex_out_n.alu_in1 | ex_out_n.alu_in2;
                    break;
                
                /* xor, xori */
                case XOR:
                    ex_out_n.alu_out = ex_out_n.alu_in1 ^ ex_out_n.alu_in2;
                    break;
                
                /* sll, slli */
                case SLL:
                    ex_out_n.alu_out = ex_out_n.alu_in1 << ex_out_n.alu_in2;
                    break;
                
                /* srl, srli */
                case SRL:
                    ex_out_n.alu_out = ex_out_n.alu_in1 >> ex_out_n.alu_in2;
                    break;
                
                /*slt, slti */
                case SLT:
                    ex_out_n.alu_out = ex_out_n.alu_in1 < ex_out_n.alu_in2;
                    break;
            }
            break;

        /* bne, beq, blt, bge (all branches compute equalities + branch in EX phase) */
        case BTYPE:
            ex_out_n.br_addr = ex_out_n.inst_addr + ex_out_n.imm; //Branches need to store jump address
            switch(ex_out_n.funct3){
                /* beq */
                case BEQ:
                    take_branch = ex_out_n.alu_in1 == ex_out_n.alu_in2;
                    break;
                
                /* bne */
                case BNE:
                    take_branch = ex_out_n.alu_in1 != ex_out_n.alu_in2;
                    break;
                
                /* blt */
                case BLT:
                    take_branch = ex_out_n.alu_in1 < ex_out_n.alu_in2;
                    break;
                
                /* bge */
                case BGE:
                    take_branch = ex_out_n.alu_in1 >= ex_out_n.alu_in2;
                    break;   
            }
            if (take_branch){
                ex_out_n.br_taken = 1; // Make sure to set br_mispredicted
            }
            break;

        /* lui */
        case LUI:
            ex_out_n.alu_out = ex_out_n.inst & 0xFFFFF000;
            break;

        /* jal */
        case JAL:
            ex_out_n.br_addr = ex_out_n.inst_addr + ex_out_n.imm;
            ex_out_n.link_addr = ex_out_n.inst_addr + 4;
            break;
        case JALR:
            ex_out_n.br_addr = ex_out_n.alu_in1 + ex_out_n.imm;
            ex_out_n.link_addr = ex_out_n.inst_addr + 4;
            break;
    }
   
    return ex_out_n;
}

/**
 * Execute stage implementation for Load and Stores
 */
struct State execute_ld_st() {
    ex_ld_st_out_n = decode_out;

    /*
     * Core-side memory client (2 states):
     *   LOOKUP   - compute address, probe cache
     *              HIT  : read/write memory[] this cycle (1-cycle hit)
     *              MISS : assert mem_req.valid, go WAIT_MEM
     *   WAIT_MEM - hold mem_req until mem_resp.ack, then read/write memory[]
     *
     * The bus only times the miss. Data still moves through memory[] here.
     */
    static enum {
        LOOKUP = 0,
        WAIT_MEM
    } client_state = LOOKUP;

    static int cache_way = -1;

    switch (client_state) {
        case WAIT_MEM:
            dmem_busy = 1;
            mem_req_n.valid = 1;

            /* mem_resp_n: same-cycle ack (memory_cycle runs before core).
             * Avoids an extra bubble after Miss_latency so miss is 9, not 10. */
            if (!mem_resp_n.ack) {
                return ex_ld_st_out;
            }        

            /* Ack received: do the DRAM access in the core (as before). */
            // dcache_update(ex_ld_st_out.mem_addr, cache_way); // moved right after the hit/miss check to capture cache_trace
            dmem_accesses++;
            if (ex_ld_st_out.opcode == ITYPE_LOAD) {
                ex_ld_st_out.mem_buffer = (unsigned int) memory[ex_ld_st_out.mem_addr];
            } else if (ex_ld_st_out.opcode == STYPE) {
                memory[ex_ld_st_out.mem_addr] = (int) ex_ld_st_out.mem_buffer;
            }

            mem_req_n.valid = 0;
            dmem_busy = 0;
            client_state = LOOKUP;
            return ex_ld_st_out;

        case LOOKUP:
        default:
            if (ex_ld_st_out_n.valid == 0 ||
                (ex_ld_st_out_n.opcode != ITYPE_LOAD && ex_ld_st_out_n.opcode != STYPE)) {
                dmem_busy = 0;
                return nop;
            }

            dmem_busy = 1;
            ex_ld_st_out_n.mem_addr = ex_ld_st_out_n.alu_in1 + ex_ld_st_out_n.alu_in2;
            cache_way = dcache_lookup(ex_ld_st_out_n.mem_addr);

            if (cache_way != -1) {
                /* HIT: address + lookup + data all in this one cycle. */
                dcache_hits++;
                dcache_update(ex_ld_st_out_n.mem_addr, cache_way);
                if (cache_trace == 1) {
                    if (ex_ld_st_out_n.opcode == ITYPE_LOAD) {
                        fprintf(fptr_mt, "@ Cycle %d, LD addr %d, hit way %d\n", cycle, ex_ld_st_out_n.mem_addr, cache_way);
                    } else if (ex_ld_st_out_n.opcode == STYPE) {
                        fprintf(fptr_mt, "@ Cycle %d, ST addr %d, hit way %d\n", cycle, ex_ld_st_out_n.mem_addr, cache_way);
                    }
                }
                dmem_accesses++;
                if (ex_ld_st_out_n.opcode == ITYPE_LOAD) {
                    ex_ld_st_out_n.mem_buffer = (unsigned int) memory[ex_ld_st_out_n.mem_addr];
                } else if (ex_ld_st_out_n.opcode == STYPE) {
                    memory[ex_ld_st_out_n.mem_addr] = (int) ex_ld_st_out_n.mem_buffer;
                }
                dmem_busy = 0;
                return ex_ld_st_out_n;
            }

            /* MISS: ask memory to wait out the DRAM latency. */
            dcache_update(ex_ld_st_out_n.mem_addr, cache_way); // update the cache state (for modeling purposes this is done here to capture cache_trace)
            if (cache_trace == 1) {
                if (ex_ld_st_out_n.opcode == ITYPE_LOAD) {
                    fprintf(fptr_mt, "@ Cycle %d, LD addr %d, miss way %d\n", cycle, ex_ld_st_out_n.mem_addr, cache_way);
                } else if (ex_ld_st_out_n.opcode == STYPE) {
                    fprintf(fptr_mt, "@ Cycle %d, ST addr %d, miss way %d\n", cycle, ex_ld_st_out_n.mem_addr, cache_way);
                }
            }
            mem_req_n.valid = 1;
            client_state = WAIT_MEM;
            return ex_ld_st_out_n;
    }
}


/**
 * Writeback stage implementation
 */
struct State writeback() {

    // load/store is still occupying the memory unit; do not commit yet
    if (dmem_busy) {
        wb_out_n = nop;
    }
    // instruction is coming from the Load/Store execute unit
    else if ((ex_out.valid == 0) && (ex_ld_st_out.valid == 1)) {
        wb_out_n = ex_ld_st_out;
    }
    // instruction is coming from the ALU execute unit
    else if ((ex_out.valid == 1) && (ex_ld_st_out.valid == 0)) {
        wb_out_n = ex_out;
    }
    else if ((ex_out.valid == 1) && (ex_ld_st_out.valid == 1)) {
        printf("ERROR: writeback cannot process valid instructions from both execute and execute_ld_st stages\n");
        abort();
    }
    // neither execute unit has a valid instruction
    else {
        wb_out_n = nop;
    }

    if (wb_out_n.valid == 1)
    {

        switch(wb_out_n.opcode)
        {
        
            // R-type, I-type arithmatic, and lui store a register value to rd //
            case RTYPE:
            case ITYPE_ARITH:
            case LUI:
                registers[wb_out_n.rd] = wb_out_n.alu_out; //All these write ALU output to register
                advance_pc(wb_out_n.inst_addr + 4);
                break;
            
            // load stores a memory value to rd //
            case ITYPE_LOAD:
                registers[wb_out_n.rd] = wb_out_n.mem_buffer; //LW writes the loaded memory to register
                advance_pc(wb_out_n.inst_addr + 4);
                break;

            case STYPE:
                advance_pc(wb_out_n.inst_addr + 4);
                break;

            
            // jal / jalr store a link address to rs (only when rs =/= 0) //
            case JAL:
            case JALR:
                advance_pc(wb_out_n.br_addr);
                if (wb_out_n.rd != 0) 
                    registers[wb_out_n.rd] = wb_out_n.link_addr; 
                break;

            case BTYPE:
            if (wb_out_n.br_taken) advance_pc(wb_out_n.br_addr);
            else advance_pc(wb_out_n.inst_addr + 4);
           

            default:
                break;
        }

    }

    

    return wb_out_n;
}

/**
 * Advance PC.
 * DO NOT MODIFY.
 */

void advance_pc(int step) {
    pc_n = step;
}