                bits 32
                default rel
                global _start

_start: 
                push edx                        ; save edx because w syscall change this reg
                lea ebx, [rel _start]           ; load _star address to calculate .text section and jmp orign entry
                mov edi, 1                      ; stdout fd
                lea esi, [rel msg]
                mov edx, 14                     ; 14 chars 
                mov eax, 4                      ; write syscall
                int 0x80
                xor ebx, ebx
                mov eax, 1
                int 0x80
                pop edx                         ; restore edx (syscall write)

                ; init decrypt
                ; edi = text_section (puntero a la sección de texto)
                ; esi = text_size (tamaño del texto)
                ; edx = key (puntero a la clave de cifrado)
                ; ecx = key_size (tamaño de la clave)
                push eax                        ; save eax
                push ebx                        ; save ebx, _start address
                push esi                        ; save esi
                push edx                        ; save edx
                push ecx                        ; save ecx
                lea eax, [rel text_dist]        ; load .text distance in eax
                sub ebx, [eax]                  ; calculate .text section address, now in ebx
                mov edi, ebx                    ; pass .text address to edi
                lea esi, [rel text_size]        ; load in esi .text section size
                mov esi, [esi]
                lea edx, [rel key]              ; load in edx decrypt key
                lea ecx, [rel key_size]         ; load in edx key size
                mov ecx, [ecx]
                xor ebx, ebx                    ; init to 0 ebx, use for conter

xor_loop:
                mov al, [edi]
                xor al, [edx + ebx]
                mov [edi], al

                inc edi
                inc ebx
                cmp ebx, ecx
                jl continuel_xor
                xor ebx, ebx

continuel_xor:
                dec esi
                jnz xor_loop
        
                pop ecx
                pop edx
                pop esi
                pop ebx
                pop eax

                lea eax, [rel distance]
                sub ebx, [eax]
                jmp ebx
        
msg             db "....WOODY....", 10
key             times 32 db "C"             
key_size        dq "ZZZZZZZZ"
text_dist       dq "OOOOOOOO"
text_size       dq "SSSSSSSS"
distance        dq "DDDDDDDD"
