.text
#Storing initial data
addi s0, zero, 0x123 # set s0 = 0x123
addi s1, zero, 0x456 # set s1 = 0x456

#Adding nop to clear the pipeline
addi zero, zero, 0 # nop
addi zero, zero, 0 # nop
addi zero, zero, 0 # nop
addi zero, zero, 0 # nop

#interlock signal coming from execute (with fwd no interlock signal as execute stage forwards the data)
add s5, s1, s0     
and s3, s0, s1
xori s6, s5, 10   



addi zero, zero, 1 # detect this change and quit simulator

.data
256: .word 20
257: .word 5402
258: .word 5302