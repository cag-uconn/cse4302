.text 
addi t0, zero, 15   # max number of memory lookups
addi t1, t0, 1      # for upper bound of loop
add s0, zero, zero  # stores 0 if the word is not found in memory, else stores 1

addi s1, zero, 255  # We want to look up this word in memory
add t2, zero, zero  # value to store memory address

loop:
addi t2, t2, 1
slt t4, t2, t1          # check if counter < upper bound
beq t4, zero, not_found # if we have serched max memory locations, quit 
lw t5, 256(t2)          # look for word
beq s1, t5, found       # found the word 
jal x0, loop            # keep looping

found:
addi s0, zero, 1   # set 1 to s0 and fall through to termination
not_found:
addi zero, zero, 1 # end the program

.data
256: .word 3
257: .word 671
258: .word 5402
259: .word 0
260: .word 4302
261: .word 15
262: .word 255
263: .word 5402
264: .word 5302
265: .word 3
266: .word 23
267: .word 1
268: .word 2024