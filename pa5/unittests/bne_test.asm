.text
lui s0, 0x8000        # set s0 = 0x08000000
addi s1, zero, 0xFFF  # set s1 = 0xFFFFFFFF
addi s11, zero, 1     # set s1 = 1 (constant)
add s2, zero, zero    # set s2 = 0 (shift counter)

loop:
slt t0, s1, s0        # set t0 = s0 < s1
bne t0, zero, end     # branch to end if t0 = zero (if previous condition is false)
srli s1, s1, 1        # shift s1 right by 1 (s1 = s1 >> 1)
addi s2, s2, 1        # increment shift counter
bne zero, s11, loop   # branch to label loop -- always branches

end:
sw s2, 256(zero)     # store shift counter in memory
sw s0, 257(zero)     # store final s0 value in memory
addi zero, zero, 1   # detect this change and quit simulator

.data
256: .word 8
257: .word 22
258: .word 42
259: .word 68
260: .word 100
261: .word 138
262: .word 182
263: .word 232
264: .word 288
265: .word 350
266: .word 418
267: .word 492
268: .word 572
269: .word 658
270: .word 750
271: .word 848
272: .word 952
273: .word 1062
274: .word 1178
275: .word 1300
276: .word 1428
277: .word 1562
278: .word 1702
279: .word 1848
280: .word 2000
281: .word 2158
282: .word 2322
283: .word 2492
284: .word 2668
285: .word 2850
286: .word 3038