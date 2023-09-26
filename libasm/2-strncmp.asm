section .text
global asm_strncmp

asm_strncmp:
	; Function prologue (stack frame setup)
	push    rbp
	mov     rbp, rsp

	; Parameters:
	; rdi = s1
	; rsi = s2
	; rdx = n

	; Initialize counter
	mov     rcx, rdx
	
compare_loop:
	; Load characters from s1 and s2 into AL and BL
	lodsb
	mov     al, [rdi]
	mov     bl, [rsi]

	; Compare AL and BL
	cmp     al, bl
	jne     not_equal

	; Check if we've reached the end of the strings or the end of the comparison
	test    al, al
	jz      equal
	loop    compare_loop

equal:
	; Strings are equal
	xor     rax, rax
	jmp     end

not_equal:
	; Strings are not equal
	sub     al, bl
	jmp     end

end:
	; Function epilogue (stack frame cleanup)
	pop     rbp
	ret

