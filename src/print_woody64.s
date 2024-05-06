; Print in stdout ....WOODY.....

section .data
  msj db '....WOODY.....', 0
  format db '%s', 0

section .text
global  print_woody

print_woody:
  lea rsi, [msj]
  call printf
  ret
