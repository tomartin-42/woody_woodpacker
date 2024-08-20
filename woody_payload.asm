                bits 64
                default rel
                global _start

_start: 
                push rdx
                push rdi
                push rsi
                push rdx
                lea rbx, [rel _start]
                push rbx                        ; Salva la dirección de _star para hacer el jump
                mov rdi, 1                      ; stdout fd
                lea rsi, [rel msg]
                mov rdx, 10                     ; 10 chars + newline
                mov rax, 1                      ; write syscall
                syscall
                
                ; Desencriptado
                xor rbx, rbx                    ; rbx = 0 (contador)
                lea rdi, [rel text_off]         ; Carga el puntero a la sección .text
                lea rsi, [rel text_size]        ; Carga el tamaño de la sección .text
xor_loop:       mov al, [rel text_off]          ; Cargar un byte de text_section en al (usar rdi como puntero)
                lea rdx, [rel key]
                add rdx, rbx
                xor al, [rdx]                   ; XOR con el byte correspondiente de la clave
                mov [rdi], al                   ; Almacena el resultade en la scción .text
                inc rdi
                inc rbx
                cmp rbx, [rel key_size]         ; Compara el indice de la clave con 
                jl continue_xor                 ; Si rbx < key_size, coninúa con la key 
                xor rbx, rbx                    ; Si no, reinicia rbx a 0 para repetir key

continue_xor:   dec rsi                         ; Decrementa el tamaño de la sección .text 
                jnz xor_loop                    ; Si no es 0, repite xor_loop 

                ; End
                pop rsi                         ; Restaura rsi
                pop rdi                         ; Restaura rdi
                pop rdx                         ; Restaura rdx (write syscall)
                pop rdx
                pop rbx                         ; Restaura rbx (necesario para jump)
                lea rax, [rel distance]
                sub rbx, [rax]
                jmp rbx
        
msg             db "..WOODY..", 10
key             db "AAAAAAAAA"
key_size        db 0x42
text_off        dq 0x1122334455667788
text_size       dq 0x1122334455667788
distance        dq 0x1122334455667788
