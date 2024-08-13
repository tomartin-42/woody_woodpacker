section .text
global encrypt

encrypt:
    ; Prologue
    push rbp                ; Guardar el valor anterior de rbp
    mov rbp, rsp            ; Establecer el nuevo rbp
    push rbx                ; Guardar el valor de rbx (usado en bucles)
    push rsi                ; Guardar el valor de rsi (puntero de texto)
    push rdi                ; Guardar el valor de rdi (puntero de clave)

    ; Parámetros de la función
    mov rsi, [rbp + 16]     ; rsi = text_section (puntero a la sección de texto)
    mov rcx, [rbp + 24]     ; rcx = text_size (tamaño del texto)
    mov rdi, [rbp + 32]     ; rdi = key (puntero a la clave de cifrado)
    mov rdx, [rbp + 40]     ; rdx = key_size (tamaño de la clave)
    xor rbx, rbx            ; rbx = 0 (contador de la clave)

    ; Bucle principal
    xor_loop:
        mov al, [rsi]       ; Cargar un byte de text_section en al
        xor al, [rdi + rbx] ; XOR con el byte correspondiente de la clave
        mov [rsi], al       ; Almacenar el resultado en la sección de texto

        inc rsi             ; Avanzar al siguiente byte de text_section
        inc rbx             ; Avanzar al siguiente byte de la clave
        cmp rbx, rdx        ; Comparar el índice de la clave con key_size
        jl continue_xor     ; Si rbx < key_size, continúa usando la clave
        xor rbx, rbx        ; Si no, reinicia rbx a 0 para repetir la clave

    continue_xor:
        loop xor_loop       ; Decrementa rcx y si no es 0, repite el bucle

    ; Epílogo
    pop rdi                 ; Restaurar el valor de rdi
    pop rsi                 ; Restaurar el valor de rsi
    pop rbx                 ; Restaurar el valor de rbx
    mov rsp, rbp            ; Restaurar el valor de rsp
    pop rbp                 ; Restaurar el valor de rbp
    ret                     ; Retornar al llamador

