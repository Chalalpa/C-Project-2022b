; dfsdf sdr4545
.entry LOOP
.entry LENGTH
.extern W 
lea r3, r4
MAIN: 	mov S1.1 		, W
	add r2, STR
	LOOP:		jmp W
	macro m1
		inc	K
		
		mov S1.2 ,#-5
	endmacro
		prn #-5
	sub r1, r4
			m1
	bne L3
	END:   hlt	
STR: .string "abcdef"
LENGTH:	.data 6, -9, 15
K:	.data	22 
S1:	.struct 8, "ab"
.extern L3