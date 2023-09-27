BITS 64

	global asm_strchr

	section .text
	;char *asm_strchr(const char *s, int c);

asm_strchr:
	; Input:
	;   RDI - Pointer to the input string (s)
	;   RSI - Integer representing the character to search for (c)
	; Output:
	;   RAX - Pointer to the first occurrence of c in s or NULL if not found

xor rax, rax             ; Clear RAX to store the result

mov rdx, rsi             ; Copy c to RDX for comparison
xor rsi, rsi             ; Clear RSI to use it as an index

.search_loop:
	mov al, byte [rdi + rsi] ; Load the next byte from the string into AL
	cmp al, 0                ; Check if we've reached the end of the string
	je .not_found            ; If it's the end, character not found
	cmp al, dl               ; Compare the current character with c
	je .found                ; If they match, we found c
	inc rsi                  ; Increment index
	jmp .search_loop         ; Continue searching

.found:
	lea rax, [rdi + rsi]     ; Calculate the direction of c in s
	ret

.not_found:
	xor rax, rax             ; Set RAX to NULL (character not found)
	ret
