# Same operation as derivative.asm, except loop unrolled 8x

# Starting address of y[n]: 256 (# of elements 32)
# Starting address of f[n]: 288 (# of elements 31)

.text
addi s0, zero, 31 # Upper bound for counter
add s1, zero, zero # Counter i: 0 -> 3
lw s3,  256(s1) # y[i] load
loop:

lw s4,  257(s1) # y[i+1] load
lw s5,  258(s1) # y[i+2] load
lw s6,  259(s1) # y[i+3] load
lw s7,  260(s1) # y[i+4] load
lw s8,  261(s1) # y[i+5] load
lw s9,  262(s1) # y[i+6] load
lw s10, 263(s1) # y[i+7] load
lw s11, 264(s1) # y[i+8] load

sub s2, s4, s2  # y[i+1] - y[i-1]
sub s3, s5, s3  # y[i+2] - y[i]
sub s4, s6, s4  # y[i+3] - y[i+1]
sub s5, s7, s5  # y[i+4] - y[i+2]
sub s6, s8, s6  # y[i+5] - y[i+3]
sub s7, s9, s7  # y[i+6] - y[i+4]
sub s8, s10, s8 # y[i+7] - y[i+5]
sub s9, s11, s9 # y[i+8] - y[i+6]

addi s1, s1, 8   # Update the counter here to remove the RAW hazard on SW / BLT!

srli s2, s2, 1  # f[n]   = ( y[i+1] - y[i-1] ) / 2
srli s3, s3, 1  # f[n+1] = ( y[i+2] - y[i] )   / 2
srli s4, s4, 1  # f[n+2] = ( y[i+3] - y[i+1] ) / 2
srli s5, s5, 1  # f[n+3] = ( y[i+4] - y[i+2] ) / 2
srli s6, s6, 1  # f[n+4] = ( y[i+5] - y[i+3] ) / 2
srli s7, s7, 1  # f[n+5] = ( y[i+6] - y[i+4] ) / 2
srli s8, s8, 1  # f[n+6] = ( y[i+7] - y[i+5] ) / 2
srli s9, s9, 1  # f[n+7] = ( y[i+8] - y[i+6] ) / 2

# (Accounting for the previously incrememnted counter) 
sw s2, 280(s1)   # f[n] store
sw s3, 281(s1)   # f[n+1] store
sw s4, 282(s1)   # f[n+2] store
sw s5, 283(s1)   # f[n+3] store
sw s6, 284(s1)   # f[n+4] store
sw s7, 285(s1)   # f[n+5] store
sw s8, 286(s1)   # f[n+6] store
sw s9, 287(s1)   # f[n+7] store

add s2, s10, zero  # y[i-1] = y[i+7]  (to avoid double loading)
add s3, s11, zero  # y[i] = y[i+8]  (to avoid double loading)
blt s1, s0, loop # Loop through whole array
sw zero, 287(s1) # Since the output length is 31, the 32'nd element is erroneous: clear it

addi zero, zero, 1 

.data 
# y[n] sequence
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
287: .word 3232