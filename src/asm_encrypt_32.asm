section .text
global asm_encrypt_32

asm_encrypt_32:
    ; Prologue
    push rbp                ; Guardar el valor anterior de rbp
    mov ebp, esp            ; Establecer el nuevo rbp
    push rbx                ; Guardar el valor de rbx (usado en bucles)

    ; Parámetros ya están en registros:
    ; rdi = text_section (puntero a la sección de texto)
    ; rsi = text_size (tamaño del texto)
    ; rdx = key (puntero a la clave de cifrado)
    ; rcx = key_size (tamaño de la clave)
    xor ebx, ebx            ; rbx = 0 (contador de la clave)

    ; Bucle principal
    xor_loop:
        mov al, [edi]       ; Cargar un byte de text_section en al (usar rdi como puntero)
        xor al, [edx + ebx] ; XOR con el byte correspondiente de la clave
        mov [edi], al       ; Almacenar el resultado en la sección de texto

        inc edi             ; Avanzar al siguiente byte de text_section
        inc ebx             ; Avanzar al siguiente byte de la clave
        cmp ebx, ecx        ; Comparar el índice de la clave con key_size
        jl continue_xor     ; Si rbx < key_size, continúa usando la clave
        xor ebx, ebx        ; Si no, reinicia rbx a 0 para repetir la clave

    continue_xor:
        dec esi             ; Decrementar el tamaño del texto (text_size)
        jnz xor_loop        ; Si no es 0, repite el bucle


    ; Epílogo
    pop rbx                 ; Restaurar el valor de rbx
    mov esp, ebp            ; Restaurar el valor de rsp
    pop rbp                 ; Restaurar el valor de rbp
    ret                     ; Retornar al llamador
