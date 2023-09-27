BITS 64

	global asm_strstr

	section .text
asm_strstr:
    ;
    push rbp
    mov rbp, rsp
    push rcx
    push rbx
    push rdx
    push r8
    push r9

    ; Inicializar registros
    xor rax, rax
    xor rcx, rcx
    xor r8, r8
    xor r9, r9

.search_loop:
    ; Copiar el índice actual en RDX
    mov rdx, rcx
    xor rbx, rbx

.search_match:
    ; Cargar el carácter actual de haystack en R8B
    ; Cargar el carácter actual de needle en R9B
    mov r8b, [rdi + rcx]
    mov r9b, [rsi + rbx]

    ; Comprobar si hemos llegado al final de needle (carácter nulo)
    cmp r9b, 0
    jz .found_needle

    ; Comparar el carácter actual de haystack con el de needle
    cmp r8b, r9b
    jne .search_break

    ; Incrementar índices y continuar buscando
    inc rcx
    inc rbx
    jmp .search_match

.search_break:
    ; Cargar el siguiente carácter en haystack
    mov r8b, [rdi + rcx]

    ; Comprobar si es el final de haystack (carácter nulo)
    cmp r8b, 0
    jz .search_end

    ; Incrementar índice en haystack y continuar buscando
    inc rcx
    jmp .search_loop

.found_needle:
    ; Calcular la dirección del inicio de needle en haystack
    mov rax, rdi
    add rax, rdx

.search_end:
    ;
    pop r9
    pop r8
    pop rdx
    pop rbx
    pop rcx
    mov rsp, rbp
    pop rbp
    ret
