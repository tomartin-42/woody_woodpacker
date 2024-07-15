        bits 64
        default rel
        global _start

_start: 
        mov rdi, 1      ; stdout fd
        lea rsi, [rel msg]
        mov rdx, 10      ; 9 chars + newline
        mov rax, 1      ; write syscall
        syscall

        mov     dl, 42         ;petit registre pour enregistrer 42 sans 0x00
        xor     eax, eax
        inc     eax
        mul     dl             ;multiplier 42 * rax (rax = 1), resultat dans rax
        mov     edi, eax       ;bouger la valeur 42 de rax à rdi
        xor     eax, eax
        mov     al, 60         ;on met le syscall exit dans rax
        syscall
        
msg    db "..WOODY..", 10
