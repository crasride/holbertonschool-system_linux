BITS 64           ; Selecciona la arquitectura de 64 bits

	global asm_strspn 

	section .text
	;size_t asm_strspn(const char *s, const char *accept);

asm_strspn:       ; Etiqueta de la función
    push rbp       ; Guarda el valor de RBP en la pila
    mov rbp, rsp   ; Establece RBP como puntero de pila

    push rcx       ; Guarda el valor de RCX en la pila (se usará para contar la longitud)
    push r8        ; Guarda el valor de R8 en la pila (se usará para comparar caracteres)
    push r9        ; Guarda el valor de R9 en la pila (se usará para cargar caracteres de las cadenas)

    xor rax, rax   ; Limpia RAX (se usará para contar la longitud)
    xor r8, r8     ; Limpia R8 (se usará para comparar caracteres)
    xor r9, r9     ; Limpia R9 (se usará para cargar caracteres)

.loop_strspn_rdi:  ; Etiqueta del bucle exterior (recorre la cadena en RDI)
    mov r8b, [rdi + rax] ; Carga el carácter actual de la cadena en RDI
    test r8b, r8b         ; Comprueba si el carácter es nulo (fin de cadena)
    jz .end                ; Si es nulo, salta a la etiqueta 'end' (fin de la función)

    xor rcx, rcx   ; Limpia RCX (se usará para recorrer la cadena en RSI)

.loop_strspn_rsi:  ; Etiqueta del bucle interior (recorre la cadena en RSI)
    mov r9b, [rsi + rcx] ; Carga el carácter actual de la cadena en RSI
    test r9b, r9b         ; Comprueba si el carácter es nulo (fin de cadena)
    jz .end                ; Si es nulo, salta a la etiqueta 'end'

    cmp r8b, r9b    ; Compara los caracteres en R8 y R9
    je .jump         ; Si son iguales, salta a la etiqueta 'break'

    inc rcx          ; Incrementa el índice de la cadena en RSI
    jmp .loop_strspn_rsi ; Vuelve al inicio del bucle interior

.jump:
    inc eax          ; Incrementa la longitud
    jmp .loop_strspn_rdi ; Vuelve al inicio del bucle exterior

.end:
    pop r9           ; Restaura el valor de R9 desde la pila
    pop r8           ; Restaura el valor de R8 desde la pila
    pop rcx          ; Restaura el valor de RCX desde la pila
    mov rsp, rbp     ; Restaura el puntero de pila original
    pop rbp          ; Restaura el valor de RBP
    ret              ; Devuelve el resultado





