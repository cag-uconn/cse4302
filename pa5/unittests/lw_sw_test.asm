.text
add t0, zero, zero  # i = 0
add t1, zero, zero  # initialize the sum to zero
add t2, zero, zero  # for second loop compare 2
add t3, zero, zero
add t5, zero, zero  # initialize temporary register to zero
add t6, zero, zero  # for sw later

lw t1, 256(t0)     # t1 = 20
lw t2, 256(t1)     # t2 = 4 
add t4, t1, t2     # t4 = 24
lw t3, 256(t4)     # t3 = 8
add t4, t4, t3     # t4 = 32
sw t4, 256(t0)     # mem[256] = 32
lw t1, 256(t0)     # t1 = 32 <-- observation of stored value 32 as $t1=0x00000020
lw t2, 256(t1)     # t2 = 5
add t4, t1, t2     # t4 = 37
sw t4, 256(t1)     # mem[288] = 37
lw t5, 256(t1)     # t5 = 37  <--- observation of stored value 37 as $t5=0x00000025

addi zero, zero, 1 # $zero register should never be updated, so detect this change and quit simulator

.data
256: .word 20
257: .word 32
258: .word 2
259: .word 2
260: .word 3
261: .word 3
262: .word 3
263: .word 3
264: .word 3
265: .word 3
266: .word 3
267: .word 3
268: .word 3
269: .word 3
270: .word 3
271: .word 3
272: .word 3
273: .word 3
274: .word 3
275: .word 3
276: .word 4
277: .word 3
278: .word 3
279: .word 3
280: .word 8
281: .word 3
282: .word 3
283: .word 3
284: .word 3
285: .word 3
286: .word 3
287: .word 3
288: .word 5

