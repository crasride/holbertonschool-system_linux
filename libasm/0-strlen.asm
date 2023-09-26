section .text
global asm_strlen

asm_strlen:
	push rbp                ; Save the base pointer
	mov rbp, rsp            ; Set up the stack frame
	push rcx                ; Save the counter register

	xor rcx, rcx            ; Initialize rcx to 0 (length counter)

.loop:
	cmp byte [rdi + rcx], 0; Compare the byte at rdi+rcx to the null terminator
	je .done                ; If it's the null terminator,Ok
	inc rcx                 ; Move to the next character
	jmp .loop               ; Repeat the loop

.done:
	mov rax, rcx            ; Move the length in rcx to rax (return value)
	pop rcx                 ; Restore counter register
	mov rsp, rbp            ; Restore stack pointer
	pop rbp                 ; Restore the base pointer
	ret                     ; Return with the length in rax
