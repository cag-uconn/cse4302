.text 
lw t0, 256(t0)     
addi zero, zero, 1 # detect this change and quit simulator
.data
256: .word 20
257: .word 30
258: .word 40