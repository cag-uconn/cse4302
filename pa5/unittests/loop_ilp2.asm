.text
lw s0, 256(a0)   #s0 = 10 --> A[0]
lw s1, 257(a0)   #s1 = 20 --> A[1]
add t0,s0,s1     #A[0]+A[1]
lw s2, 258(a0)   #s2 = 30 --> A[2]
lw s3, 259(a0)   #s3 = 40 --> A[3]
add t1,s2,s3     #A[2]+A[3]
add t0,t0,t1     #A[0]+A[1]+A[2]+A[3]
lw s4, 260(a0)   #s4 = 50 --> A[4]
lw s5, 261(a0)   #s5 = 60 --> A[5]
add t2,s4,s5     #A[4]+A[5]
lw s6, 262(a0)   #s6 = 70 --> A[6]
lw s7, 263(a0)   #s7 = 80 --> A[7]
add t3,s6,s7     #A[6]+A[7]
add t2,t2,t3     #A[4]+A[5]+A[6]+A[7]
add t4,t0,t2     #A[0]+A[1]+A[2]+A[3]+A[4]+A[5]+A[6]+A[7]
sw t4, 267(a0)   # D[2]




addi zero, zero, 1        

.data
256: .word 10
257: .word 20
258: .word 30
259: .word 40
260: .word 50
261: .word 60
262: .word 70
263: .word 80