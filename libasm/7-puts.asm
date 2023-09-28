global asm_puts

    ; Llamar a la funcion exterior
    extern asm_strlen

    section .text
    ; size_t asm_puts(const char *str);

asm_puts:
    push rbp              ; Guarda el valor de rbp en la pila
    mov rbp, rsp          ; Establece rbp como el nuevo puntero de pila
    push rdi              ; Guarda el valor de rdi en la pila

    ; Llama a la función asm_strlen
    call asm_strlen       ; Llama a la función asm_strlen
    mov rdx, rax          ; Resultado de asm_strlen (longitud la cadena) a rdx
    mov rax, 1            ; Establece valor de rax a 1 (syscall para la consola)
    mov rdi, 1            ; Establece el descriptor de archivo (stdout) en rdi
    mov rsi, [rsp]        ; De la cadena (puntero) desde la pila a rsi

    ; Realiza la llamada al sistema para escribir en la consola
    syscall
    mov rax, rdx          ; Valor de rdx (longitud original) de vuelta a rax
    pop rdi               ; Restaura el valor de rdi desde la pila
    mov rsp, rbp          ; Restaura el puntero de pila original
    pop rbp               ; Restaura el valor de rbp original

    ; Return asm_puts
    ret
