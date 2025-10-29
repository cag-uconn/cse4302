.text
addi s0, zero, 256     # starting address of data
lw a0, 0(s0)           # load argument from memory         
jal ra, reverse        # procedure call

sw a0, 1(s0)           # store output in memory
addi zero, zero, 1     # terminate
#reverse the endianness (eg 0x 01 23 45 67 --> 0x 67 45 23 01)
reverse:
add t0, zero, zero     # t0 = 0
add t1, zero, zero     # t1 = 0
add t2, zero, zero     # t2 = 0
add t3, zero, zero     # t3 = 0
add t4, zero, zero     # t4 = 0
addi t5, zero, 24      # t5 = 24 (constant)
add t6, zero, zero     # temp output register
loop:
sub t1, t5, t0         # t1 = 24 - t0
srl t2, a0, t0         # t2 = a0 >> t0
andi t3, t2, 0xFF      # t3 = 8-LSB of t2
sll t4, t3, t1         # t4 = t3 << t1
or t6, t6, t4          # t6 |= t4
addi t0, t0, 8         # t0 += 8
bge t5, t0, loop       # loop while t0 <= 24

add a0, t6, zero       # set return value
jalr zero, ra, 0       # output is set so return

.data
256: .word 0x43022023



