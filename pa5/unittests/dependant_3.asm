.text
#Storing initial data
addi s0, zero, 0x123 # set s0 = 0x123
addi s1, zero, 0x456 # set s1 = 0x456

#Adding nop to clear the pipeline
addi zero, zero, 0 # nop
addi zero, zero, 0 # nop
addi zero, zero, 0 # nop
addi zero, zero, 0 # nop


#interlock signal coming from memory  (with fwd no interlock signal as memory stage forwards the data)
xori a0, s1, 10    # a0 = 0x0000045C
sub a1, s1, s0     # a1 = 0x00000333
sw s1, 256(s7)     # mem[256] = 0x00000456
srl a2, a0, s0     # a2 = 0x0000008B


addi zero, zero, 1 # detect this change and quit simulator

.data
256: .word 20
257: .word 5402
258: .word 5302