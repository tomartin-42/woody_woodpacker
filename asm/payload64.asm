
bits 64
default rel

section .text

global payload64_start
global payload64_end
global payload64_text_delta
global payload64_entry_delta
global payload64_text_size
global payload64_key_size
global payload64_woody_str
global payload64_key

payload64_start:
    jmp payload64_code

payload64_text_delta:
    dq 0

payload64_entry_delta:
    dq 0

payload64_text_size:
    dq 0

payload64_key_size:
    dq 0

payload64_woody_str:
    db "....WOODY.....", 10

payload64_key:
    times 64 db 0
    
payload64_code:

    ; Preservo todos los registros
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

       ; Print "...WOODY..." 
    mov rax, 1                          ; syscall write
    mov rdi, 1                          ; std_out
    lea rsi, [rel payload64_woody_str]  ; "....WOODY....." string
    mov rdx, 15                         ; string len
    syscall

        ; calcular .text en runetime
    lea r12, [rel payload64_start]      ; Dirección runtime donde empieza el payload
    mov r13, [rel payload64_text_delta] ; Distancia desde el payload hasta .text suele ser negativo
    add r13, r12                        ; Dirección runtime de .text
    mov r14, [rel payload64_text_size]  ; Tamaño de .text
        
        ; Calcular las páginas ocupadas por la sección .text
        ; Es necesario para usar mprotect()
    mov r8, r13          ; Addr inicio de .text
    and r8, -4096        ; Redondea hacia abajo al inicio de página
    lea r9, [r13 + r14]  ; Addr final de .text
    add r9, 4095         ; Redondea hacia arriba al final de página
    and r9, -4096
    sub r9, r8           ; r9 = bytes que debe usar mprotect()

        ; Necesitamos cambiar permisos de .text para poder descifrar
        ; Syscall mprotect() 
        ; rdi = addr página de inicio .text
        ; rsi = bytes a cambiar
        ; rdx = Flags
    mov rax, 10        ; mprotect() syscall
    mov rdi, r8        ; Pagína de inicio de .text
    mov rsi, r9        ; Bytes que deben cambiar sus flags
    mov rdx, 3         ; PROT_READ | PROT_WRITE
    syscall
    test rax, rax
    js payload64_error

        ;; Rutina de descifrado
    xor rbx, rbx                        ; rbx = 0 (contador de la clave)
    lea rdx, [rel payload64_key]        ; rdx = puntero a la key
    mov rdi, r13                        ; puntero al inicio exacto de .text
    mov rcx, [rel payload64_key_size]   ; tamaño de la clave
    mov rsi, r14                        ; bytes restantes de .text
    xor_loop:
        mov al, [rdi]               ; Mueve a al 1 byte de .text
        xor al, [rdx + rbx]         ; XOR
        mov [rdi], al               ; Mueve a .text el byte resultante de XOR

        inc rdi                     ; Incremeta el puntero a .text (siguiente byte)
        inc rbx                     ; Incremeta el puntero a key (siguiente byte)
        cmp rbx, rcx                ; compara valor rbx con key_size
        jb continue_xor
        xor rbx, rbx                ; reinicia contado de clave
    continue_xor:
        dec rsi                     ; decrementa el tamaño de .text
        jnz xor_loop                ; si no 0, repite el bucle

        ; Ahora restauramos permisos de .text despues del descifrado
        ; Syscall mprotect() 
        ; rdi = addr página de inicio .text
        ; rsi = bytes a cambiar
        ; rdx = Flags
    mov rax, 10     ; mprotect() syscall
    mov rdi, r8     ; Pagína de inicio de .text
    mov rsi, r9     ; Bytes que deben cabiar sus flags
    mov rdx, 5      ; PROT_READ | PROT_EXEC
    syscall
    test rax, rax
    js payload64_error

        ; Restauro registros para delegar la ejecución al entry original
        ; y no se vea afectado
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    
        ; Salto al entry original
    lea rax, [rel payload64_start]
    add rax, [rel payload64_entry_delta] ; Debería ser negativo
    jmp rax

payload64_error:
    mov rax, 60     ; exit() syscall
    mov rdi, 1      ; EXIT_FAILURE
    syscall

payload64_end:
