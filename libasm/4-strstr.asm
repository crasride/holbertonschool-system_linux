section .text
global asm_strstr

asm_strstr:
    ; Input:
    ;   RDI - Pointer to the haystack (string to search in)
    ;   RSI - Pointer to the needle (substring to find)
    ; Output:
    ;   RAX - Pointer to the first occurrence of 'needle' in 'haystack'

    xor rax, rax                  ;
    cmp byte [rsi], 0             ;
    je .found_needle              ;

    .outer_loop:
        mov rcx, rdi              ;
        .inner_loop:
            mov al, [rcx]         ;
            mov dl, [rsi]         ;
            cmp dl, 0             ;
            je .found_needle_loop ;
            cmp al, dl            ;
            jne .next_char        ;
            inc rcx               ;
            inc rsi               ;
            jmp .inner_loop       ;

        .next_char:
            inc rdi               ;
            cmp byte [rdi], 0     ;
            jne .outer_loop       ;
            xor rax, rax          ;
            ret

        .found_needle_loop:
            lea rax, [rdi - 1]    ;
            ret

    .found_needle:
        lea rax, [rdi]             ;
        ret
