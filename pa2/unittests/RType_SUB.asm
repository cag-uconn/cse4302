.text 
addi t0, t0, 999    
addi t1, t1, 2133   
sub t2, t0 t1       
addi zero, zero, 1 # detect this change and quit simulator
.data
2048: .word 4302 
2049: .word 5402 
2050: .word 2023 