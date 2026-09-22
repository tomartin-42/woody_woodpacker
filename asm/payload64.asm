
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
    syscal

        ; calcular .text en runetime
    lea r12, [rel payload64_start]      ; Dirección runtime donde empieza el payload
    mov r13, [rel payload64_text_delta] ; Distancia desde el payload hasta .text suele ser negativo
    add r13, r12                        ; Dirección runtime de .text
    mov r14, [rel payload64_text_size]  ; Tamaño de .text
        
        ; Calcular número de paginas de la sección .text
        ; Es necesario para usar mprotect()
    mov r8, r13         ; Addr inicio de .text
    and r8, 0x1000      ; Redondea hacia abajo la addr de .text
    lea r9, [r13, r14]  ; Addr final de .text
    add r9, 0x1000      ; Añado 0x1000 para saltar si o si a la siguiente página
    and r9, 0x1000      ; Redondea hacia arriba la addr final de .text    
    sub r9, r8          ; r9 = bytes que debe usar mprotect()

        ; Necesitamos cambiar permisos de .text para poder descifrar
        ; Syscall mprotect() 
        ; rdi = addr página de inicio .text
        ; rsi = bytes a cambiar
        ; rdi = Flags
    mov rax, 10        ; mprotect() syscall
    mov rdi, r8        ; Pagína de inicio de .text
    mov rsi, r9        ; Bytes que deben cambiar sus flags
    mov rdx, 3         ; PROT_READ | PROT_WRITE
    syscall

    ;; Rutina de descifrado

        ; Ahora restauramos permisos de .text despues del descifrado
        ; Syscall mprotect() 
        ; rdi = addr página de inicio .text
        ; rsi = bytes a cambiar
        ; rdi = Flags
    mov rax, 10     ; mprotect() syscall
    mov rdi, r8     ; Pagína de inicio de .text
    mov rsi, r9     ; Bytes que deben cabiar sus flags
    mov rdx, 5      ; PROT_READ | PROT_EXEC

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

payload64_end:
