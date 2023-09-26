BITS 64

global asm_strncmp
section .text

asm_strncmp:
	; Configure stack frame
	push rbp            ; Guarda el valor de rbp en la pila
	mov rbp, rsp        ; Establece rbp como el puntero de marco actual
	mov rcx, 0          ; Inicializa rcx (contador) a cero

compare:
	xor rax, rax        ; Limpia rax (se usará para el resultado)
	xor rbx, rbx        ; Limpia rbx (se usará para comparar los bytes)
	cmp ecx, edx        ; Compara el contador con la longitud n
	je end_comparison   ; Si son iguales, finaliza la comparación

	mov al, byte [rdi + rcx]  ; Carga el byte de s1 en AL
	mov bl, byte [rsi + rcx]  ; Carga el byte de s2 en BL
	cmp al, bl           ; Compara los bytes
	je equal_chars       ; Si son iguales, salta a la etiqueta equal

	jl less_chars        ; Si no son iguales y al es menor que bl, salta a less
	jg greater_chars     ; Si no son iguales y al es mayor que bl, salta a greater

equal_chars:
	; The characters are the same, check if it is a null character
	cmp al, 0            ; Comprueba si hemos llegado al final de una de las cadenas
	je end_comparison    ; Si es así, finaliza la comparación
	inc rcx              ; Incrementa el contador
	jmp compare          ; Vuelve a la etiqueta compare para comparar el siguiente byte

less_chars:
	; Return a negative value (s1 < s2)
	mov rax, -1          ; Si al es menor que bl, establece el resultado en -1 (menor)
	jmp end_comparison   ; Salta a end_comparison para finalizar la comparación

greater_chars:
	; Return a positive value (s1 > s2)
	mov rax, 1           ; Si al es mayor que bl, establece el resultado en 1 (mayor)
	jmp end_comparison   ; Salta a end_comparison para finalizar la comparación

end_comparison:
	; clear stack frame
	mov rsp, rbp         ; Restaura rsp al valor original
	pop rbp              ; Restaura rbp desde la pila
	ret                  ; Retorna con el resultado en rax

