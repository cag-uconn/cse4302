.text
addi t6, zero, 10   # t6 = 10 (upper bound)
addi t1, zero, 0    # t1 = 0 (F_0)
addi t2, zero, 1    # t2 = 1 (F_1)
jal x0, fibonacci

fibonacci:          
add t3, t2, t1      # F_n = F_(n-1) + F_(n-2) 
add t1, zero, t2    # F_(n-2) = F_(n-1) 
add t2, zero, t3    # F_(n-1) = F_n
addi t5, t5, 1      # i++
bne t5, t6, fibonacci 
addi zero, zero, 1  # zero register should never be updated, so detect this change and quit simulator

.data
256: .word 10
257: .word 10
258: .word 20
259: .word 30
260: .word 40
261: .word 50
262: .word 60
263: .word 70
264: .word 80
265: .word 90
266: .word 100