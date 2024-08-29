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
        
msg             db "..WOODY..", 10
key             db "AAAAAAAA"
key_size        db 0x1122334455667788
text_off        dq 0x1122334455667788
text_size       dq 0x1122334455667788
distance        dq 0x1122334455667788
