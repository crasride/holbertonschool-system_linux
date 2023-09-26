BITS 64

	global asm_strcmp

	section .text
	;int asm_strcmp(const char *s1, const char *s2);
	;Compara dos cadenas y devuelve un número entero que representa el resultado
	;de la comparación (negativo si la primera cadena es menor que la segunda,
	;positivo si es mayor y cero si son iguales).


asm_strcmp:
	; Prólogo de la función (configurar marco de pila)
	push rbp         ; Guarda el valor del registro de base (rbp) en la pila
	mov rbp, rsp     ; Establece rbp igual a rsp (el puntero de pila actual)

	; Manejo de parámetros
	mov rdi, rdi     ; rdi contiene s1 (este paso no hace nada)
	mov rsi, rsi     ; rsi contiene s2 (este paso no hace nada)

	; Comparar cadenas en un bucle
compare_loop:
	; Cargar caracteres de s1 y s2 en los registros al y bl
	mov al, byte [rdi]  ; Carga el byte en la dirección apuntada por rdi en al
	mov bl, byte [rsi]  ; Carga el byte en la dirección apuntada por rsi en bl

	; Comparar caracteres
	cmp al, bl      ; Compara los caracteres en al y bl
	je equal_chars  ; Salta a equal_chars si son iguales
	jl less_chars   ; Salta a less_chars si al es menor que bl
	jg greater_chars ; Salta a greater_chars si al es mayor que bl

equal_chars:
	; Los caracteres son iguales, verificar si es un carácter nulo
	cmp al, 0       ; Compara el carácter en al con nulo (0)
	je strings_equal ; Si son iguales, las cadenas son iguales

	; Moverse a los caracteres siguientes
	inc rdi         ; Incrementa el puntero rdi para apuntar al siguiente carácter de s1
	inc rsi         ; Incrementa el puntero rsi para apuntar al siguiente carácter de s2
	jmp compare_loop ; Vuelve a compare_loop para continuar la comparación

less_chars:
	; Devolver un valor negativo (s1 < s2)
	mov rax, -1     ; Carga -1 en rax para indicar que s1 es menor que s2
	jmp end_comparison ; Salta a end_comparison para finalizar la comparación

greater_chars:
	; Devolver un valor positivo (s1 > s2)
	mov rax, 1      ; Carga 1 en rax para indicar que s1 es mayor que s2
	jmp end_comparison ; Salta a end_comparison para finalizar la comparación

strings_equal:
	; Devolver 0 (s1 == s2)
	xor rax, rax    ; Establece rax en 0 para indicar que las cadenas son iguales

end_comparison:
	; Epílogo de la función (limpiar marco de pila)
	pop rbp          ; Restaura el valor de rbp desde la pila
	ret             ; Retorna de la función, devolviendo el valor en rax

