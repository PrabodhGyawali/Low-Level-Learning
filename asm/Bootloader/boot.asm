bits 16
org 0x7c00

mov si, 0

print:
	mov ah, 0x0e
	mov al, [hello + si]
	int 0x10 		; interrupt trigger
	add si, 1
	cmp byte [hello + si], 0
	jne print
jmp $

hello:
	db "Hello, world!", 0


times 510 - ($ - $$) db 0
dw 0xAA55			; x86 magic  

