	.global _syscall_readSector
	.global _syscall_printString
	.global _syscall_readString
	.global _syscall_readFile
	.global _syscall_executeProgram
	.global _syscall_terminate
	.global _syscall_clearScreen

_syscall_readSector:
	push bp
	mov bp,sp
	mov ax,#2
	mov bx,[bp+4]
	mov cx,[bp+6]
	int #0x21
	pop bp
	ret

_syscall_printString:
	push bp
	mov bp,sp
	mov ax,#0
	mov bx,[bp+4]
	int #0x21
	pop bp
	ret

_syscall_readString:
	push bp
	mov bp,sp
	mov ax,#1
	mov bx,[bp+4]
	int #0x21
	pop bp
	ret

_syscall_readFile:
	push bp
	mov bp,sp
	mov ax,#3
	mov bx,[bp+4]
	mov cx,[bp+6]
	int #0x21
	pop bp
	ret

_syscall_executeProgram:
	push bp
	mov bp,sp
	mov ax,#4
	mov bx,[bp+4]
	mov cx,[bp+6]
	int #0x21
	pop bp
	ret

_syscall_terminate:
	mov ax,#5
	int #0x21
	ret

_syscall_clearScreen:
	mov ax,#10
	int #0x21
	ret
