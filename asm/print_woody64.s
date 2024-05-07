; Print in stdout ....WOODY.....
extern printf

section .data
  msg db '....WOODY.....\n', 0

section .text
global  print_woody

print_woody:
  mov rax, 1
  mov rdi, 1
  mov rsi, msg
  mov rdx, 15
  syscall
  ret


