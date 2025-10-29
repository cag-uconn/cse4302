/**
 * University of Connecticut
 * CSE 4302 / CSE 5302 / ECE 5402: Computer Architecture
 * Fall 2025
 * 
 * 
 * riscy-uconn: instruction_map.h
 * 
 * DO NOT MODIFY THIS FILE
 * 
 */

#pragma once

/* Some helpful defines for decoding instructions */
#define bit_7            0x00000080
#define bit_20           0x00100000
#define bit_31           0x80000000
#define bit_2_downto_0   0x00000007
#define bit_4_downto_0   0x0000001F
#define bit_6_downto_0   0x0000007F
#define bit_10_downto_7  0x00000780
#define bit_11_downto_7  0x00000F80
#define bit_11_downto_8  0x00000F00
#define bit_19_downto_12 0x000FF000
#define bit_30_downto_21 0x7FE00000
#define bit_30_downto_25 0x7E000000
#define bit_31_downto_12 0xFFFFF000
#define bit_31_downto_20 0xFFF00000
#define bit_31_downto_25 0xFE000000

/* Opcode Bits */
#define RTYPE        0x33
#define ITYPE_ARITH  0x13
#define ITYPE_LOAD   0x3
#define STYPE        0x23
#define BTYPE        0x63
#define LUI          0x37
#define JAL          0x6F
#define JALR         0x67

/* Funct3 Bits */
/* R and I-Type Arithmetic */
#define ADD_SUB      0b000
#define SUB          0b000
#define SLT          0b010
#define SLL          0b001
#define SRL          0b101
#define AND          0b111
#define OR           0b110
#define XOR          0b100
/* I-type JALR and Load, S-type, and B-type */
#define LW_SW        0b010
#define BEQ          0b000
#define BNE          0b001
#define BLT          0b100
#define BGE          0b101

/* R-type Funct7s */
#define ADD_F7       0x0
#define SUB_F7       0x20