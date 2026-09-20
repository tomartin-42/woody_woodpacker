
bits 64
default rel

section .text

global payload64_start
global payload64_end
global payload64_text_delta
global payload64_entry_delta
global payload64_text_size
global payload64_key_size
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

payload64_key:
    times 64 db 0

payload64_code:
    ; Código del payload

payload64_end:
