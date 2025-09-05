.text
addi t0, t0, 999    
srli t1, t0, 1      
addi zero, zero, 1 # detect this change and quit simulator
.data
2048: .word 4302 
2049: .word 5402 
2050: .word 2023 