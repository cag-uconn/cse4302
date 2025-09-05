# y(t)  = 3*t^2 + 5*t
# y'(t) = 6*t + 5
# Estimate y(t) as a discrete sequence: y[n] = 3*n^2 + 5*n ; n:{1 <= n <= 32}
# Derivative approximation: f[n] = (y[n+1] - y[n-1]) / 2

# Starting address of y[n]: 256 (# of elements 32)
# Starting address of f[n]: 288 (# of elements 31)

.text
addi s0, zero, 31  # Upper bound for counter
add s1, zero, zero # Counter i: 0 -> 31

lw s3, 256(s1)  # load y[i] temp for later

loop:
lw s4, 257(s1)   # y[i+1] load
sub s5, s4, s2   # y[i+1] - y[i-1]
srli s5, s5, 1   # f[n] = ( y[i+1] - y[i-1] ) / 2 
sw s5, 288(s1)   # f[n] store
add s2, s3, zero # y[i-1] = y[i] 
add s3, s4, zero # y[i] = y[i+1]  (to avoid double loading)
addi s1, s1, 1   # i++
blt s1, s0, loop # Loop through whole array

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