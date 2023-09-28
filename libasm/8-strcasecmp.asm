section .text
global asm_strcasecmp

asm_strcasecmp:
    ; Argumentos:
    ; rdi: puntero a la primera cadena (s1)
    ; rsi: puntero a la segunda cadena (s2)
    xor rax, rax ; Inicializa el resultado a 0 (iguales)

.loop:
    ; Carga los caracteres de s1 y s2 en al y bl
    mov al, byte [rdi]
    mov bl, byte [rsi]

    ; Convierte los caracteres a minúsculas si es necesario
    cmp al, 'A'
    jl .check_second
    cmp al, 'Z'
    jg .check_second
    add al, 32 ; Convierte a minúsculas (ASCII 'A' a 'a')

.check_second:
    cmp bl, 'A'
    jl .compare_chars
    cmp bl, 'Z'
    jg .compare_chars
    add bl, 32 ; Convierte a minúsculas (ASCII 'A' a 'a')

.compare_chars:
    ; Compara los caracteres
    cmp al, bl
    jne .done ; Si son diferentes, termina

    ; Verifica si llegamos al final de las cadenas
    cmp al, 0
    je .done ; Si sí, son iguales

    ; Avanza a los siguientes caracteres
    inc rdi
    inc rsi
    jmp .loop

.done:
    ; Devuelve el resultado en rax
    ret
