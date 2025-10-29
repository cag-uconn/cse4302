.text
lui s0, 0xFFFFF    # s0 = all '1' in 20 MSB
ori s0, s0, 0xFFF  # s1 = all '1' in MSB (constant)

# Words to test
addi s1, zero, 2023
addi s2, zero, -2023

# Convert t1 to +4302
xor t1, s2, s0     # flip all bits in s2
addi t2, t1, 1     # add 1 to t2

and t3, t2, s1     # Check mutual bits
bne t3, s1, fail
xori s0, s0, 0xAAA # Imprint pattern on s0 

fail:
addi zero, zero, 1 # terminate

.data
256: .word 10
257: .word 10