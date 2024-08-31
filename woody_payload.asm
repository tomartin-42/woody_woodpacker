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
                push rax
                push rbx
                lea rax, [rel text_off]
                sub rbx, [rax]
                pop rbx
                pop rax
                lea rax, [rel distance]
                sub rbx, [rax]
                jmp rbx
        
msg             db "..WOODY..", 10
key             db "AAAAAAAA"
key_size        dq "ZZZZZZZZ"
text_off        dq "OOOOOOOO"
text_size       dq "SSSSSSSS"
distance        dq "DDDDDDDD"
