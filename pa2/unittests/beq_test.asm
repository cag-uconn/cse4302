.text
addi s0, zero, 1     # set s0 = 0
addi s1, zero, 0x8   # set s1 = 0b1000
add s2, zero, zero   # set s2 = 0 (shift counter)

loop:
beq s0, s1, end       # if s0 = s1, branch to end label
slli s0, s0, 1        # shift s0 left by 1 (s0 = s0 << 1)
addi s2, s2, 1        # increment shift counter
beq zero, zero, loop  # branch to label loop -- always branches

end:
sw s2, 256(zero)      # store shift counter in memory
sw s0, 257(zero)      # store final s0 value in memory
addi zero, zero, 1    # detect this change and quit simulator

.data
256: .word 10
257: .word 32
258: .word 2