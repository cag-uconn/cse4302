# Testing 4-way SA pseudo-LRU replacement policy

.text 
addi s0, zero, 16  # Distance between addresses to access
add s1, zero, zero # Offset from starting data address

# Load first data block from memory (starting address = 0x100000000) -> cold miss
lw s2, 256(s1)
lw s3, 257(s1)
lw s4, 258(s1)
lw s5, 259(s1)

# Increment sw address
add s1, s1, s0

# Store data block in memory  (starting address = 0x100010000) -> cold miss
sw s2, 256(s1) 
sw s3, 257(s1)
sw s4, 258(s1)
sw s5, 259(s1)

# Increment sw address
add s1, s1, s0

# Store data block in memory in reverse order (starting address = 0x100100000) -> cold miss
sw s5, 256(s1)
sw s4, 257(s1)
sw s3, 258(s1)
sw s2, 259(s1)

# Increment sw address
add s1, s1, s0

# Load the original data block to different registers (starting address = 0x100000000) -> cache hit!
lw s6, 256(zero)
lw s7, 257(zero)
lw s8, 258(zero)
lw s9, 259(zero)

# Store the data block in shuffled order (starting address = 0x100110000) -> cold miss
sw s8, 256(s1)
sw s9, 257(s1)
sw s6, 258(s1)
sw s7, 259(s1)

# Increment sw address
add s1, s1, s0

# Operate on the data before storing it in memory (starting address = 0x101000000) -> conflict miss
add t0, s2, s9
add t1, s3, s8
add t2, s4, s7
add t3, s5, s6 
sw t0, 256(s1)
sw t1, 257(s1)
sw t2, 258(s1)
sw t3, 259(s1)

# Load circular shifted from original addr (starting address = 0x100000000) -> ...miss? ...hit?
lw s5, 256(zero)
lw s2, 257(zero)
lw s3, 258(zero)
lw s4, 259(zero)

addi zero, zero 1 # Terminate

.data 
256: .word 0xA
257: .word 0xB
258: .word 0xC
259: .word 0xD