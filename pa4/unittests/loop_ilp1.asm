.text
lw s0, 256(a0)   #s0 = 10 --> A[0]
addi s1,s0,2     #2+A[0]
addi s2,s0,3     #3+A[0]
addi s3,s0,4     #4+A[0]
sw s1, 259(a0)   # B[0]
sw s2, 260(a0)   # C[0]
sw s3, 261(a0)   # D[0]

lw s0, 257(a0)   #s0 = 20 --> A[1]
addi s1,s0,2     #2+A[1]
addi s2,s0,3     #3+A[1]
addi s3,s0,4     #4+A[1]
sw s1, 262(a0)   # B[1]
sw s2, 263(a0)   # C[1]
sw s3, 264(a0)   # D[1]

lw s0, 258(a0)   #s0 = 20 --> A[2]
addi s1,s0,2     #2+A[2]
addi s2,s0,3     #3+A[2]
addi s3,s0,4     #4+A[2]
sw s1, 265(a0)   # B[2]
sw s2, 266(a0)   # C[2]
sw s3, 267(a0)   # D[2]




addi zero, zero, 1        

.data
256: .word 10
257: .word 20
258: .word 30