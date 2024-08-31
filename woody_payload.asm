                bits 64
                default rel
                global _start

_start: 
                push rdx                        ; save rdx because w syscall change this reg
                lea rbx, [rel _start]           ; load _star address to calculate .text section and jmp orign entry
                mov rdi, 1                      ; stdout fd
                lea rsi, [rel msg]
                mov rdx, 10                     ; 9 chars + newline
                mov rax, 1                      ; write syscall
                syscall
                pop rdx                         ; restore rdx (syscall write)

                ; init decrypt
                ; rdi = text_section (puntero a la sección de texto)
                ; rsi = text_size (tamaño del texto)
                ; rdx = key (puntero a la clave de cifrado)
                ; rcx = key_size (tamaño de la clave)
                push rax                        ; save rax
                push rbx                        ; save rbx, _start address
                push rsi                        ; save rsi
                push rdx                        ; save rdx
                push rcx                        ; save rcx
                lea rax, [rel text_dist]        ; load .text distance in rax
                sub rbx, [rax]                  ; calculate .text section address, now in rbx
                mov rdi, rbx                    ; pass .text address to rdi
                lea rsi, [rel text_size]        ; load in rsi .text section size
                mov rsi, [rsi]
                lea rdx, [rel key]              ; load in rdx decrypt key
                lea rcx, [rel key_size]         ; load in rdx key size
                mov rcx, [rcx]
                xor rbx, rbx                    ; init to 0 rbx, use for conter

xor_loop:
                mov al, [rdi]
                xor al, [rdx + rbx]
                mov [rdi], al

                inc rdi
                inc rbx
                cmp rbx, rcx
                jl continuel_xor
                xor rbx, rbx

continuel_xor:
                dec rsi
                jnz xor_loop
        
                pop rcx
                pop rdx
                pop rsi
                pop rbx
                pop rax

                lea rax, [rel distance]
                sub rbx, [rax]
                jmp rbx
        
msg             db "..WOODY..", 10
key             db "AAAAAAAA"
key_size        dq "ZZZZZZZZ"
text_dist       dq "OOOOOOOO"
text_size       dq "SSSSSSSS"
distance        dq "DDDDDDDD"
