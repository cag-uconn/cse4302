/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 1: NonPipelined Simulator
 * 
 * riscv-uconn: decode_fields.h
 * 
 * DO NOT MODIFY THIS FILE  
 */

 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
* Decode Feilds
*/
void decode_fields(struct State *ptr)
{
    
    unsigned int inst = ptr->inst;
    ptr->opcode  =  inst & bit_6_downto_0;
    ptr->funct3  = (inst >> 12) & bit_2_downto_0; // 0x7;
    ptr->funct7  = (inst >> 25) & bit_6_downto_0; // 0x7F;
    ptr->rd      = (inst >> 7) & bit_4_downto_0;  // 0x1F;
    ptr->rs1     = (inst >> 15) & bit_4_downto_0; // 0x1F;
    ptr->rs2     = (inst >> 20) & bit_4_downto_0; // 0x1F;

    switch(ptr->opcode)
    {
        case JALR:
        case ITYPE_ARITH:
        case ITYPE_LOAD:
        ptr->imm = (signed)(inst & bit_31_downto_20) >> 20; // 0xFFF0000
        break;

        case STYPE:
            ptr->imm = ((signed)(inst & bit_31_downto_25) >> 20) | // 0xFE00000
                                    ((inst & bit_11_downto_7) >> 7); // 0x0000780
        break;

        case BTYPE:
            ptr->imm = ((signed)(inst & bit_31) >> 19)           | // 0x80000000
                                    ((inst & bit_30_downto_25) >> 20) | // 0x7E000000
                                    ((inst & bit_11_downto_8) >> 7)   | // 0x00000F00
                                    ((inst & bit_7) << 4);              // 0x00000080
        break;

        case JAL:
            ptr->imm = ((signed)(inst & bit_31) >> 11)           | // 0x80000000
                                    ((inst & bit_30_downto_21) >> 20) | // 0x7FE00000
                                    ((inst & bit_20) >> 9)            | // 0x00100000
                                     (inst & bit_19_downto_12);         // 0x000FF000
        break;    

    }
}
