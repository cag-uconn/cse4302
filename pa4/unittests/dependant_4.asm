.text
#Storing initial data
addi s0, zero, 0x123 # set s0 = 0x123
addi s1, zero, 0x456 # set s1 = 0x456

#Adding nop to clear the pipeline
addi zero, zero, 0 # nop
addi zero, zero, 0 # nop
addi zero, zero, 0 # nop
addi zero, zero, 0 # nop


#interlock signal coming from writeback (with fwd no interlock signal as writeback stage forwards the data)
lw a2, 256(a1)     # a2 = 0x00000014
srl s3, s1, s0     # s3 = 0x0000008A
slli s4, s0, 738   # s4 = 0x0000048C
lui s5, 409        # s5 = 0x00199000
sll a3, a2, s0     # a3 = 0x000000A0


addi zero, zero, 1 # detect this change and quit simulator

.data
256: .word 20
257: .word 5402
258: .word 5302