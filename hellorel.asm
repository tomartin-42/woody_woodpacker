                bits 64
                default rel
                global _start

_start: 
                lea rbx, [rel _start] ; get my entry point
                mov rdi, 1      ; stdout fd
                lea rsi, [rel msg]
                mov rdx, 10      ; 9 chars + newline
                mov rax, 1      ; write syscall
                syscall
                mov rax, [rel distance]
                sub rbx, rax
                jmp rbx
                ret
                ;xor     eax, eax
                ;mov     al, 60         ;on met le syscall exit dans rax
                ;syscall
        
msg             db "..WOODY..", 10
distance        dq 0x000000000001d390
