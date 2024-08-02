                bits 64
                default rel
                global _start

_start: 
                push rdx
                lea rbx, [rel _start]
                mov rdi, 1      ; stdout fd
                lea rsi, [rel msg]
                mov rdx, 10      ; 9 chars + newline
                mov rax, 1      ; write syscall
                syscall
                pop rdx
                lea rax, [rel distance]
                sub rbx, [rax]
                jmp rbx
                ;ret 
                ;xor     rax, rax
                ;mov     al, 60         
                ;syscall
        
msg             db "..WOODY..", 10
distance        dq 0x000000000001d390
