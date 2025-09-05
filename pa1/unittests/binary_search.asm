.text

# Load base address of array, size, and target
addi t0, zero, 256     # t0 = Base address of array
addi t1, zero, 8       # t1 = Number of elements (n)
addi t2, zero, 7       # t2 = Target value to search
addi s11, zero, 267    # s11 = address to store target index

# Initialize binary search pointers
addi t3, zero, 0       # t3 = low = 0
add  t4, t1, zero      # t4 = high = n

    binary_search:
    # Compute mid = low + (high - low) / 2
    add  t5, t4, t3        # t5 = high + low
    srli t5, t5, 1         # t5 = (high + low) / 2
    add  t6, zero, t5      # t6 = mid
    
    # Load array[mid]
    add  s7, t6, t0        # t7 = address of array[mid+base_address]
    lw   s8, 0(s7)         # t8 = array[mid]

    # Compare array[mid] with target
    beq  s8, t2, found     # If array[mid] == target, jump to found
    blt  s8, t2, go_right  # If array[mid] < target, search right

    # Update high = mid 
    add t4, zero, t6        # high = mid
    jal x0, binary_search

go_right:
# Update low = mid
add t3, zero, t6         # low = mid
jal x0, binary_search

found:
# Store the result (index of target)
sw   s8, 0(s11)        # result = mid
jal x0, exit


exit:
addi zero, zero, 1 # $zero register should never be updated, so detect this change and quit simulator


.data
256: .word 1
257: .word 3
258: .word 5
259: .word 7
260: .word 9
261: .word 11
262: .word 13
263: .word 15
264: .word 8
265: .word 7
266: .word 11
267: .word 0
