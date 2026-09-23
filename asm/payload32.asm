bits 32

section .text

global payload32_start
global payload32_end
global payload32_text_delta
global payload32_entry_delta
global payload32_text_size
global payload32_key_size
global payload32_woody_str
global payload32_key

payload32_start:
    jmp payload32_code

payload32_text_delta:char
    dd 0

payload32_entry_delta:
    dd 0

payload32_text_size:
    dd 0

payload32_key_size:
    dd 0

payload32_woody_str:
    db "....WOODY.....", 10

payload32_key:
    times 64 db 0

payload32_code:
    ; Conserva el estado que el loader entrega al entry point original.
    pushfd
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    push ebp

    ; i386 no tiene direccionamiento relativo a EIP: call/pop obtiene la base.
    call payload32_get_base
payload32_get_base:
    pop ebp
    sub ebp, payload32_get_base - payload32_start

    ; write(1, "....WOODY.....\n", 15)
    mov eax, 4
    mov ebx, 1
    lea ecx, [ebp + payload32_woody_str - payload32_start]
    mov edx, 15
    int 0x80

    ; Reconstruye la dirección runtime y el tamaño de .text.
    mov esi, [ebp + payload32_text_delta - payload32_start]
    add esi, ebp
    mov edi, [ebp + payload32_text_size - payload32_start]

    ; Amplía el rango de .text hasta límites de página de 4096 bytes.
    mov ebx, esi
    and ebx, -4096
    lea ecx, [esi + edi]
    add ecx, 4095
    and ecx, -4096
    sub ecx, ebx

    ; mprotect(page_start, page_size, PROT_READ | PROT_WRITE)
    mov eax, 125
    mov edx, 3
    int 0x80
    test eax, eax
    js payload32_error

    ; Conserva el rango de páginas mientras reutiliza ebx y ecx para el XOR.
    push ebx
    push ecx
    xor ebx, ebx
    lea edx, [ebp + payload32_key - payload32_start]
    mov ecx, [ebp + payload32_key_size - payload32_start]

payload32_xor_loop:
    mov al, [esi]
    xor al, [edx + ebx]
    mov [esi], al
    inc esi
    inc ebx
    cmp ebx, ecx
    jb payload32_continue_xor
    xor ebx, ebx

payload32_continue_xor:
    dec edi
    jnz payload32_xor_loop

    ; Recupera el rango y restaura los permisos de ejecución de .text.
    pop ecx
    pop ebx
    mov eax, 125
    mov edx, 5
    int 0x80
    test eax, eax
    js payload32_error

    pop ebp
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    popfd

    ; Recupera de nuevo la base sin alterar la pila y salta al entry original.
    call payload32_get_entry_base
payload32_get_entry_base:
    pop eax
    sub eax, payload32_get_entry_base - payload32_start
    add eax, [eax + payload32_entry_delta - payload32_start]
    jmp eax

payload32_error:
    mov eax, 1
    mov ebx, 1
    int 0x80

payload32_end:
