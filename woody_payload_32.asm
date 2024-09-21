                bits 32
                default rel
                global _start

_start: 
                push esp
                push eax
                push ebx
                push ecx
                push edx
                call $ + 5
                pop esi
                sub esi, 10
                ;push esi                        ; save esi _start address
                push edx                        ; save edx because write syscall change this reg
                mov ecx, esi                    
                add ecx, 105 
                mov ebx, 1                      ; output fd
                mov edx, 14                     ; 14 chars 
                mov eax, 4                      ; write syscall
                int 0x80                        
                pop edx                         ; restore edx (syscall write)
                ; End write ....WOODY....
                ;
                ; init decrypt ------------------------------------
                ; edi = text_section (puntero a la sección de texto)
                ; eax = text_size (tamaño del texto)
                ; edx = key (puntero a la clave de cifrado)
                ; ecx = key_size (tamaño de la clave)
                ; ebx = conter
                ; push eax                        ; save eax
                ; push edx                        ; save edx
                ; push ecx                        ; save ecx
                ; push ebx
                lea eax, [esi + 155]            ;; load .text distance in eax
                mov edi, esi
                sub edi, [eax]                  ; calculate .text section address
                lea eax, [esi + 159]            ;; load in eax .text section size
                mov eax, [eax]
                lea edx, [esi + 119]            ; load in edx decrypt key
                lea ecx, [esi + 151]            ;; load in edx key size
                mov ecx, [ecx]
                xor ebx, ebx                    ; init to 0 ebx, use for conter

xor_loop:
                push eax                        ; save text_size
                mov al, [edi]
                xor al, [edx + ebx]
                mov [edi], al

                inc edi
                inc ebx
                cmp ebx, ecx
                jl continuel_xor
                xor ebx, ebx

continuel_xor:
                pop eax
                dec eax                         ; decrement eax .text section size
                jnz xor_loop
        

                lea eax, [esi + 163]            ;
                sub esi, [eax]
                pop edx
                pop ecx
                pop ebx
                pop eax
                pop esp
                call esi
                ret
        
msg             db "....WOODY....", 10
key             times 32 db "C"             
key_size        db "ZZZZ"
text_dist       db "OOOO"
text_size       db "SSSS"
distance        db "DDDD"
