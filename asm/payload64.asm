
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
    ; Código del payload
    ; Código del payload
    ; Código del payload
    ; Restauro todos los registros
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
