.text
addi a2, zero, 2    # argument 0 = 2
addi a3, zero, 3    # argument 1 = 3
addi a4, zero, 4    # argument 2 = 4
addi a5, zero, 5    # argument 3 = 5
jal ra, diffofsums  # call procedure
add s0, a0, zero    # y = returned value
addi zero, zero, 1  # zero register should never be updated, so detect this change and quit simulator
  
diffofsums:
add t0, a2, a3      # $t0 = f + g
add t1, a4, a5      # $t1 = h + i
sub s0, t0, t1      # result = (f+g)-(h+i)
add a0, s0, zero    # put return value in a0
jalr zero, ra, 0    # return to caller

.data
256: .word 10
257: .word 10
