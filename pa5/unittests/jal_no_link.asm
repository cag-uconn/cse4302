.text
add t0, zero, zero      # iterator i = 0
add t2, zero, zero      # initialize a memory pointer to zero
add t3, zero, zero      # initialize temporary register to zero
add t4, zero, zero      # initialize temporary register to zero
jal x0, jump_test1      # Jump to procedure "jump_test1"

jump_test2:
lw a1, 256(t2)          # Load a1 = 2, Mem[256] = 2, 2 in simulator
addi t2, t2, 1          # Add 1 to the pointer to access Mem[2049]
lw a2, 256(t2)          # Load a2 = 10, Mem[257] = 10, 10 in simulator
jal x0, end             # Jump to procedure "end"

jump_test1:
add t5, zero, zero      # initialize temporary register to zero
add t6, zero, zero      # initialize temporary register to zero
jal x0, jump_test2      # Jump to procedure "jump_test2"

end:
addi zero, zero, 1      # zero register should never be updated, so detect this change and quit simulator

# --- Start of the Memory Layout ---

.data
256: .word 2
257: .word 10

