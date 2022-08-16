.entry LOOP
.entry LENGTH
MAIN:		mov  #3,r4
			add		#5,r3
LOOP: 		jmp		END
			macro m1
				mov		S1.2 ,r3
				inc		r1
			endmacro

			macro m2
				mov		S1.1 ,r3
				jmp		r1
			endmacro
			prn		#-5
			sub		r1, r4
			not r2
			m1
			bne		LOOP
			m2
END: 		rts
STR: 		.string "jbfkdbvk"
LENGTH:		.data	-5,67
K: 			.data 5
S1: 		.struct  8, "ab"