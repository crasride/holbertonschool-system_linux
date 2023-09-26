global asm_strncmp
section .text

asm_strncmp:
    push rbp
    mov rbp, rsp

    xor rax, rax  ; RAX se usará para devolver el resultado
    xor rcx, rcx  ; RCX se usará como contador

.compare_loop:
    mov al, [rdi + rcx]  ; Carga el byte de s1 en AL
    mov bl, [rsi + rcx]  ; Carga el byte de s2 en BL
    cmp al, bl           ; Compara los bytes
    jne .not_equal       ; Si no son iguales, salta a .not_equal

    ; Si ambos bytes son iguales y no hemos llegado a n, continuamos
    inc rcx
    cmp rcx, rdx
    jl .compare_loop

    ; Si llegamos a n o al final de una de las cadenas, son iguales
    cmp rcx, rdx
    je .equal

.not_equal:
    ; Si los bytes son diferentes o uno de los punteros llega al final, determina el resultado
    mov rax, 1   ; RAX = 1 (mayor)
    jl .end
    mov rax, -1  ; RAX = -1 (menor)

.equal:
    ; Si las cadenas son iguales, RAX ya está configurado en 0

.end:
    mov rsp, rbp
    pop rbp
    ret

