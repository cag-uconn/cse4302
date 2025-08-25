RISC-V Assembler
==============

An assembler for a subset of the RISC-V like instruction set architecture

# How to use
The assembler will take a file written in assembly language as input on the command line and will produce an output file containing the RISC-V machine code. The input file should be in ASCII text. Each line in the input assembly file contains either a mnemonic, a section header (such as .data) or a label (jump or branch target). The maximum length of a line is 4 bytes. Section headers such as .data and .text should be in a line by themselves with no other assembly mnemonic. Similarly, branch targets such as loop: will be on a line by themselves with no other assembly mnemonic. The input assembly file should only contain one data section and one text section. The first section in the file will be the .text section, followed by the .data section.

The assembler supports the following instruction set:

R-Type
- add
- sub
- and
- or
- xor
- slt
- sll
- srl

I-Type
- lw
- jalr
- addi
- andi
- ori
- xori
- slti
- slli
- srli

S-Type
- sw

B-Type
- beq
- bne
- blt
- bge

U-Type
- lui

J-Type
- jal

# Run
    to compile the assembler
    $ ./make
    
    to run the assembler on a nop.asm assembly file to write machine code in nop.out
    $ ./assembler nop.asm nop.out