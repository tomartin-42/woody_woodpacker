                bits 64
                default rel
                global _start

_start: 
                push rdx
                lea rbx, [rel _start]
                mov rdi, 1                      ; stdout fd
                lea rsi, [rel msg]
                mov rdx, 10                     ; 10 chars
                mov rax, 1                      ; write syscall
                syscall
                pop rdx

                push rbx
                push rdi
                push rsi
                push rdx
                push rcx
                push rax


                lea rbx, [rel _start]
                mov rbx, [rbx]
                lea rax, [rel text_off]
                sub rbx, [rax]
                mov rdi, rbx                    ; obten la direccion de .text

                xor rbx, rbx                    ; rbx = 0 (contador)
                ;lea rdi, [rel text_off]         ; carga en rdi offset de .text
                lea rdx, [rel key]              ; carga en rdx key
                lea rcx, [rel key_size]         ; carga en rcx key_size
                lea rsi, [rel text_size]        ; carga en rsi el tamaño de .text

xor_loop:
                mov al, [rdi]                   ; Carga un byte de la seccion .text en al
                xor al, [rdx + rbx]             ; XOR con el byte de la clave
                mov [rdi], al                   ; Almacena el byte desencriptado en .text
                inc rdi
                inc rbx
                cmp rbx, rcx                    ; Compara rbx (contador) con indice de la clave
                jl contiune_xor                 ; Si rbx < rcx continua
                xor rbx, rbx                    ; Si no, reinicia rbx a 0

contiune_xor: 
                dec rsi
                jnz xor_loop                    ; Si no es 0, repite bucle xor_loop

_jmp:           
                pop rax
                pop rcx
                pop rdx
                pop rsi
                pop rdi
                pop rbx
                lea rax, [rel distance]
                sub rbx, [rax]
                jmp rbx
        
msg             db "..WOODY..", 10
key             db "AAAAAAAA"
key_size        db 0x43
text_off        dq 0x1122334455667788
text_size       dq 0x1122334455667788
distance        dq 0x1122334455667788
