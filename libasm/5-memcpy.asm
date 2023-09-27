BITS 64

	section .text

	global asm_memcpy
	;void *asm_memcpy(void *dest, const void *src, size_t n);
	;Descripción: Esta función copia 'n' bytes desde la dirección de origen 
	;(src) a la dirección de destino (dest).
	; - Entradas:
	;   RDI: Puntero a la dirección de destino (dest).
	;   RSI: Puntero a la dirección de origen (src).
	;   RDX: Tamaño en bytes a copiar (n).
	; - Salida:
	;   RAX: Puntero a la dirección de destino (dest).

asm_memcpy:
    push rbp                ; Guardar el valor actual de RBP
    mov rbp, rsp            ; Establecer RBP como el nuevo puntero de marco
    xor rcx, rcx            ; Inicializar RCX a cero (contador)

    ; Verificar si n (RDX) es cero y, en ese caso, retornar dest (RDI)
    cmp rdx, 0
    je .done

.copy_loop:
    ; Comparar el contador (RCX) con n (RDX)
    cmp rcx, rdx
    je .done                ; Si RCX igual a RDX, terminar el bucle

    mov al, byte [rsi + rcx] ; Cargar el byte actual desde src (RSI) en AL
    mov byte [rdi + rcx], al ; Almacenar el byte en dest (RDI)

    inc rcx                 ; Incrementar el contador
    jmp .copy_loop          ; Saltar al inicio del bucle para copiar el siguiente byte

.done:
    mov rax, rdi            ; Retornar el puntero a la dirección de destino (dest)
    pop rbp                 ; Restaurar el valor anterior de RBP
    ret                     ; Retornar


