section .text
global asm_encrypt

asm_encrypt:
    ; Prologue
    push rbp                ; Guardar el valor anterior de rbp
    mov rbp, rsp            ; Establecer el nuevo rbp
    push rbx                ; Guardar el valor de rbx (usado en bucles)

    ; Parámetros ya están en registros:
    ; rdi = text_section (puntero a la sección de texto)
    ; rsi = text_size (tamaño del texto)
    ; rdx = key (puntero a la clave de cifrado)
    ; rcx = key_size (tamaño de la clave)
    xor rbx, rbx            ; rbx = 0 (contador de la clave)

    ; Bucle principal
    xor_loop:
        mov al, [rdi]       ; Cargar un byte de text_section en al (usar rdi como puntero)
        xor al, [rdx + rbx] ; XOR con el byte correspondiente de la clave
        mov [rdi], al       ; Almacenar el resultado en la sección de texto

        inc rdi             ; Avanzar al siguiente byte de text_section
        inc rbx             ; Avanzar al siguiente byte de la clave
        cmp rbx, rcx        ; Comparar el índice de la clave con key_size
        jl continue_xor     ; Si rbx < key_size, continúa usando la clave
        xor rbx, rbx        ; Si no, reinicia rbx a 0 para repetir la clave

    continue_xor:
        dec rsi             ; Decrementar el tamaño del texto (text_size)
        jnz xor_loop        ; Si no es 0, repite el bucle


    ; Epílogo
    pop rbx                 ; Restaurar el valor de rbx
    mov rsp, rbp            ; Restaurar el valor de rsp
    pop rbp                 ; Restaurar el valor de rbp
    ret                     ; Retornar al llamador
