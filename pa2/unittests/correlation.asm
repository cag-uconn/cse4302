.text 
addi a1, zero, 1000 # constant 1
addi a2, zero, 0xA  # constant 2
addi a3, zero, 0xB  # constant 3
addi a4, zero, 0xC  # constant 4

# Call f(a) 11 times with a = {1500, 500, 1400, 600, 1300, 700, 1200, 800, 1100, 900, 1000} 

# funct call 1 
addi a0, zero, 1500
jal ra, f
add s2, a0, zero 

# funct call 2
addi a0, zero, 500
jal ra, f
add s3, a0, zero 

# funct call 3 
addi a0, zero, 1400
jal ra, f
add s4, a0, zero 

# funct call 4 
addi a0, zero, 600
jal ra, f
add s5, a0, zero 

# funct call 5 
addi a0, zero, 1300
jal ra, f
add s6, a0, zero 

# funct call 6 
addi a0, zero, 700
jal ra, f
add s7, a0, zero 

# funct call 7 
addi a0, zero, 1200
jal ra, f
add s8, a0, zero 

# funct call 8 
addi a0, zero, 800
jal ra, f
add s9, a0, zero 

# funct call 9
addi a0, zero, 1100
jal ra, f
add s8, a0, zero 

# funct call 10 
addi a0, zero, 900
jal ra, f
add s9, a0, zero 

# funct call 11 
addi a0, zero, 1000
jal ra, f
add s8, a0, zero 

# goto end
jal x0, end

f:
add t0, zero, zero # t0 = 0

bge a0, a1, skip1  # b1: if a0 >= 1000, branch T (skip assignment) --> "if (a0 < 1000) t0 = 0xA"
addi t0, zero, 0xA 

skip1:
slli t1, t0, 12    # t1 = t0 << 12
bne t0, a2, skip2  # b2: if t0 != 0xA, branch T (skip assignment) --> "if (t0 == 0xA) t0 = 0xB"
addi t0, zero, 0xB

skip2:
slli t2, t0, 8     # t2 = t0 << 8
bne t0, a3, skip3  # b3: if t0 != 0xB, branch T (skip assignemnt) --> "if (t0 == 0xB) t0 = 0xC"
addi t0, zero, 0xC 

skip3:
slli t3, t0, 4     # t3 = t0 << 4
bne t0, a4, skip4  # b4: if t1 != 0xC, branch T (skip assignment) --> "if (t0 == 0xC) t0 = 0xD"
addi t0, zero, 0xD 

skip4:
or t4, t0, t1
or t5, t2, t3 
or a0, t4, t5      # set the output
jalr x0, ra, zero  # return

end:
addi zero, zero, 1 # terminate

.data
256: .word 4302
257: .word 5402
258: .word 5302