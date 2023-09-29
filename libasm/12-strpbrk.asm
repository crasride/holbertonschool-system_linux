BITS 64

	global asm_strpbrk

	section .text
	; char *asm_strpbrk(const char *s, const char *accept);

asm_strpbrk:
	push rbp         ; Guarda el valor de RBP en la pila
	mov rbp, rsp     ; Establece RBP como el puntero de pila

	mov rax, rdi     ; Copia el puntero de la cadena 's' a RAX
	xor rcx, rcx     ; Limpia RCX (se usará para recorrer la cadena 's')

.loop_s:
	movzx r8, byte [rax + rcx]  ; Carga el carácter actual de la cadena 's' en R8
	test r8, r8                 ; Comprueba si el carácter es nulo (fin de cadena)
	jz .not_found               ; Si es nulo, no se encontró ningún carácter de 'accept'

	mov rsi, rsi                ; Copia el puntero de la cadena 'accept' a RSI
	xor r9, r9                  ; Limpia R9 (se usará para recorrer la cadena 'accept')

.loop_accept:
	movzx r10, byte [rsi + r9] ; Carga el carácter actual de la cadena 'accept' en R10
	test r10, r10              ; Comprueba si el carácter es nulo (fin de cadena)
	jz .next_character         ; Si es nulo, pasa al siguiente carácter de 's'

	cmp r8, r10                ; Compara el carácter de 's' con el carácter de 'accept'
	je .found                  ; Si son iguales, se encontró un carácter de 'accept'

	inc r9                     ; Incrementa el índice de 'accept'
	jmp .loop_accept           ; Vuelve al inicio del bucle de 'accept'

.found:
	mov rax, rax                ; Copia el puntero de la cadena 's' a RAX (resultado encontrado)
	add rax, rcx                ; Añade la posición actual en 's' al puntero de 's'
	pop rbp                     ; Restaura el valor de RBP
	ret                         ; Devuelve el resultado (puntero al carácter encontrado)

.next_character:
	inc rcx                      ; Incrementa el índice de 's'
	jmp .loop_s                  ; Vuelve al inicio del bucle de 's'

.not_found:
	xor rax, rax                ; Si no se encuentra ningún carácter de 'accept', devuelve NULL
	pop rbp                     ; Restaura el valor de RBP
	ret                         ; Devuelve NULL
