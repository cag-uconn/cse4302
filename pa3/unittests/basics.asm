.text
addi s0, zero, 0x123 # set s0 = 0x123
addi s1, zero, 0x456 # set s1 = 0x456

# Test some basic R-Type / I-Type instructions
add s2, s0, s1     # s2 = 0x579
sub s3, s1, s0     # s3 = 0x333
or s4, s3, s2      # s4 = 0x77B
srli s5, s4, 1     # s5 = 0x3BD
and s6, s5, s4     # s6 = 0x339
xor s7, s3, s0     # s7 = 0x210
slli s8, s7, 3     # s8 = 0x1080 
xori s9, s3, 0xFFF # s9 = 0xFFFFFCCC 
slt s10, s3, s6    # s10 = 1
sll t0, s0, s10    # t0 = 0x246
srl t1, s1, s10    # t1 = 0x22B
addi zero, zero, 1 # detect this change and quit simulator

.data
256: .word 4302
257: .word 5402
258: .word 5302