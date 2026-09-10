.text

addi t0, zero, 256    # Base address of matrix A
addi t1, zero, 320    # Base address of matrix B
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


addi zero, zero, 1 # $zero register should never be updated, so detect this change and quit simulator

.data
256: .word 2
257: .word 2
258: .word 2
259: .word 2
260: .word 0
261: .word 0
262: .word 0
263: .word 0
264: .word 0
265: .word 0
266: .word 0
267: .word 0
268: .word 0
269: .word 0
270: .word 0
271: .word 0
272: .word 0
273: .word 0
274: .word 0
275: .word 0
276: .word 0
277: .word 0
278: .word 0
279: .word 0
280: .word 0
281: .word 0
282: .word 0
283: .word 0
284: .word 0
285: .word 0
286: .word 0
287: .word 0
288: .word 0
289: .word 0
290: .word 0
291: .word 0
292: .word 0
293: .word 0
294: .word 0
295: .word 0
296: .word 0
297: .word 0
298: .word 0
299: .word 0
300: .word 0
301: .word 0
302: .word 0
303: .word 0
304: .word 0
305: .word 0
306: .word 0
307: .word 0
308: .word 0
309: .word 0
310: .word 0
311: .word 0
312: .word 0
313: .word 0
314: .word 0
315: .word 0
316: .word 0
317: .word 0
318: .word 0
319: .word 0
320: .word 1
321: .word 1
322: .word 1
323: .word 1



