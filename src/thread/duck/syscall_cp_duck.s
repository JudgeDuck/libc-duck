.global __syscall_duck

.global __syscall_cp_asm
.hidden __syscall_cp_asm
.type   __syscall_cp_asm,@function
__syscall_cp_asm:
jmp __syscall_duck

.global __syscall
.hidden __syscall
.type   __syscall,@function
__syscall:
jmp __syscall_duck
