.text


addi s10, zero, 10     # Number of iterations around this to train BHSR
iterations:
addi t0, zero, 0
addi t1, zero, 0
addi t2, zero, 0 
addi s11, zero, 0
addi s1, zero, 0 
addi s9, s9, 1        # itr_id ++


addi t0, zero, 256    # Base address of matrix A
addi t1, zero, 260    # Base address of matrix B
addi t2, zero, 264    # Base address of matrix C

addi s11, zero, 2     # Matrix Size

addi s0, zero, 0      # s0 = i = 0 (outer loop index for rows of A)

outer_loop:
addi s1, zero, 0      # s1 = j = 0 (inner loop index for columns of B)

    inner_loop:
    addi s2, zero, 0        # s2 = k = 0 (dot product accumulator)
    addi t6, zero, 0        # reset 

        dot_product_loop:
        # Load A[i][k]  
        addi a3, zero, 0        # reset 
        addi a0, zero, 0        # Accumulator for C[i][j]
        add s5, zero, zero      # reset variable
        slli s5, s0, 1          # memory indexing for A - i >> 1
        add s5, s5, s2          # memory indexing for A - i >> 1 + k   
        add s5, s5, t0          # memory indexing for A - base_address + (i >> 1 + k)   
        lw a1, 0(s5)          # A[i][k] = A[s0][s2] = mem[i>> 1 + k] 

        # Load B
        add s6, zero, zero      # reset variable
        slli s6, s2, 1          # memory indexing for B - k >> 1
        add s6, s6, s1          # memory indexing for B - k >> 1 + j   
        add s6, s6, t1          # memory indexing for B - base_address + (k >> 1 + j)   
        lw a2, 0(s6)          # A[i][k] = A[s0][s2] = mem[i>> 1 + k] 

        # Do multiplication
            multiply:
            add a0,a0,a1        
            addi a3,a3,1
            bne a3, a2, multiply 

        # Accumulate the product into t6
        add  t6, t6, a0      # Accumulate the result

        # Increment k and check if k < 2
        addi s2, s2, 1        # k++
        bne  s2, s11, dot_product_loop

    # Store the result in C
    add s7, zero, zero      # reset variable
    slli s7, s0, 1          # memory indexing for C - i >> 1
    add s7, s7, s1          # memory indexing for C - i >> 1 + j   
    add s7, s7, t2          # memory indexing for C - base_address + (i >> 1 + j)
    sw t6, 0(s7)            # store in C memory
    

    # Increment j and check if j < 2
    addi s1, s1, 1        # j++
    bne  s1, s11, inner_loop

# Increment i and check if i < 2
addi s0, s0, 1        # i++
bne  s0, s11, outer_loop

bne s9, s10, iterations


addi zero, zero, 1 # $zero register should never be updated, so detect this change and quit simulator

.data
256: .word 1
257: .word 1
258: .word 1
259: .word 1
260: .word 2
261: .word 2
262: .word 2
263: .word 2
264: .word 0
265: .word 0
266: .word 0
267: .word 0



