.text
addi a0, zero, 2         # argument 0 = 2
addi a1, zero, 3         # argument 1 = 3
addi a2, zero, 4         # argument 2 = 4
addi a3, zero, 5         # argument 3 = 5
addi t4, zero, 36        # t4 = address where procedure starts
jalr ra, t4, 0           # call procedure

addi zero, zero, 1       # this instruction should be skipped!
add s0, a0, zero         # y = returned value
jalr x0, x0, 56          # go to end

#procedure 
add t0, a0, a1          # $t0 = f + g
add t1, a2, a3          # $t1 = h + i
sub s0, t0, t1          # result = (f+g)-(h+i)
add a0, s0, zero        # put return value in $v0
jalr x0, ra, 4          # return to the caller, and SKIP the instruction following "jalr ra, t4, 0" by including +4 offset

#end
addi zero, zero, 1        # $zero register should never be updated, so detect this change and quit simulator

.data
256: .word 10
257: .word 10
