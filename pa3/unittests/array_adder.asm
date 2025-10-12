.text
addi s9, zero, 10
loop:
lw s2, 257(s1)   #s2 = mem[257 + s1]     
lw s3, 273(s1)   #s3 = mem[273 + s1]
addi s1, s1, 1   #s1++
lw s4, 257(s1)   #s4 = mem[257 + s1]
lw s5, 273(s1)   #s5 = mem[273 + s1]
addi s1, s1, 1   #s1++

add s6, s2, s3   
add s7, s4, s5   

sw s6, 256(s8)   
addi s8, s8, 1
sw  s7, 256(s8) 
addi s8, s8, 1
bne s1, s9, loop

addi zero, zero, 1        

.data
256: .word 10
257: .word 10
258: .word 20
259: .word 30
260: .word 40
261: .word 50
262: .word 60
263: .word 70
264: .word 80
265: .word 90
266: .word 90
267: .word 100
268: .word 3
269: .word 3
270: .word 3
271: .word 3
272: .word 4
273: .word 11
274: .word 10
275: .word 20
276: .word 30
277: .word 40
278: .word 50
279: .word 60
280: .word 70
281: .word 80
282: .word 90
283: .word 100
284: .word 3
285: .word 3
286: .word 3
287: .word 3
288: .word 3
289: .word 5


