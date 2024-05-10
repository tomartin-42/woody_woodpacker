; Print ...WOODY...
global _start

section .data
msg db "...WOODY...", 10   ; Agregamos el valor ASCII de salto de línea (10) al final de la cadena

section .text

_start:
    mov   rdi, 1    ; stdout fd
    mov   rsi, msg
    mov   rdx, 13   ; woody string len
    mov   rax, 1    ; syscall write
    syscall

    xor   rdi, rdi  ; return 0
    mov   rax, 60   ; syscall exit
    syscall

