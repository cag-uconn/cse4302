.text
addi s0, zero, 20  # s0 = multiplier
addi s1, zero, 23  # s1 = multiplicand
add s2, zero, zero # s2 = 0 (counter) 
addi s3, zero, 1   # s3 = 1 (constant)
slli s1, s1, 8     # s1 = s1 << 8

mult:
and t0, s0, s3         # test lsb of multiplier
beq zero, t0, skip_add # only do an add if LSB is 1, so skip if 0
add s0, s0, s1         # add multiplier and multiplicand
skip_add:
srli s0, s0, 1         # s0 = s0 >> 1
addi s2, s2, 1         # s2++
slti t0, s2, 8         # check if s2 < 8
bne t0, zero, mult     # if it is, keep looping
add s10, s0, zero      # s10 = s0
addi zero, zero, 1     # $zero register should never be updated, so detect this change and quit simulator

.data
256: .word 10
257: .word 10
