.text
add s0, zero, zero    # set s0 = 0 
addi s1, zero, 5      # set s1 = 5

addloop:
addi s0, s0, 1        # s0++
blt s0, s1, addloop   # loop if s0 < s1

#swap s0 and s1
add s10, s0, zero     # move value of s0 to s10
add s0, s0, s1        # s0 = s0 + s1 = 5 + 5 = 10
sub s1, s0, s1        # s1 = s0 - s1 = 10 - 5 = 5

subloop:
addi s0, s0, -1       # s0--
bge s0, s1, subloop   # loop if s0 >= s1

add s11, s0, zero     # move value of s0 to s11

end:
sw s10, 256(zero)     # store first value of s0 in memory
sw s11, 257(zero)     # store final s0 value in memory
addi zero, zero, 1     # detect this change and quit simulator

.data
256: .word 10
257: .word 32
258: .word 2