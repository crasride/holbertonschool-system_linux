section .text
global asm_strncmp

BITS 64

asm_strncmp:
    push rbp
    mov rbp, rsp
    push rcx

    ; Check if n is zero
    mov rax, rdx
    test rax, rax
    je equal

.loop:
    mov rax, rdi
    mov rcx, rsi
    movzx rax, byte [rax]
    movzx rcx, byte [rcx]
    cmp al, 0x0
    je equal
    cmp al, cl
    jne not_equal
    inc rdi
    inc rsi
    dec rcx
    jnz .loop

equal:
    xor rax, rax
    jmp end

not_equal:
    cmp al, cl
    jl less
    mov rax, 1
    jmp end

less:
    mov rax, -1
    jmp end

end:
    pop rcx
    pop rbp
    ret
