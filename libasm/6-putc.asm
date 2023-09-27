BITS 64

global asm_putc

section .text
; This function prints a single character to standard output (stdout) using
; the write system call. The character to print is passed as an argument in
; the RDI registry. The function returns the total number of bytes written to
; stdout, which will always be 1, since a single character is printed.
;
; Tickets:
; - RDI: Register that contains the character to print.
;
; Departures:
; - RAX: Register containing the total number of bytes written to the output
; standard, which will always be 1.


asm_putc:
    ;Saves the current RBP value and sets RBP as the new frame pointer
    push rbp
    mov rbp, rsp

    ; Saves current RDI, RSI and RDX values
    push rdi
    push rsi
    push rdx

    ; syscall number for write
    mov rax, 1

    ; Places the character (passed in RDI) the memory address pointed to by RSI
    mov [rsi], rdi

    ; Arguments for the write syscall
    mov rdi, 1              ; Standard Output File Descriptor
    mov rdx, 1              ; Input data length (single character)

    ; Call the write syscall to print the character
    syscall

    ; Restores previous RDX, RSI and RDI values
    pop rdx
    pop rsi
    pop rdi

    ; Restores the value of RSP and RBP
    mov rsp, rbp
    pop rbp

    ; Return
    ret
