section .note.GNU-stack noalloc noexec nowrite progbits

SECTION .data
	message1: db "Enter the first number: ",10,  0
	message2: db "Enter the second number: ", 10, 0
	op_msg: db "Enter operator: ", 10, 0
	result_msg: db "Result: %lld", 10, 0
	formatin: db "%lld", 0
	formatout: db "%lld", 10, 0
	formatc: db " %c", 0
	valid_ops: db "+-*/|&", 0

	; Error messages
	err_divide: db "Divide by 0 error!", 10, 0
	err_op: db "<operator> invalid: use operator in {+, -, *, /, %, &, |}", 10, 0 
	err_overflow: db "Arithmetic overflow occured!", 10, 0
	err_input: db "Invalid input! Please enter valid numbers", 10, 0

SECTION .bss
	buffer resb 2
	integer1 resq 1
	integer2 resq 1
	operator resb 1
	; Error codes
	error_code resb 1

SECTION .text
	global main
	extern scanf
	extern printf

main:
	push rbp
	mov rbp, rsp

	; call printf
	mov rdi, message1
	xor rax, rax
	call printf

	; First scanf call
	mov rdi, formatin
	mov rsi, integer1
	xor rax, rax
	call scanf
	
	; Ask operator to use
	mov rdi, op_msg
	xor rax, rax
	call printf

	mov rdi, formatc
	mov rsi, operator
	xor rax, rax
	call scanf	

	; Ask for 2nd number 
	mov rdi, message2
	xor rax, rax
	call printf

	mov rdi, formatin
	mov rsi, integer2
	xor rax, rax
	call scanf

	movzx rax, byte [operator]	; (move with zero extension) byte to AL

	cmp al, '+'
	je op_add
	cmp al, '-'
	je op_sub
	cmp al, '*'
	je op_mult
	cmp al, '/'
	je op_div
	cmp al, '%'
	je op_mod
	cmp al, '&'
	jmp op_and
	cmp al, '|'
	jmp op_or
	; Operator Err
	jmp err_op_routine
	
op_add:
	mov rax, [integer1]
	add rax, [integer2]
	jmp print_result

op_sub:
	mov rax, [integer1]
	sub rax, [integer2]
	jmp print_result

op_mult:
	mov rax, [integer1]
	imul qword [integer2]
	jmp print_result

op_div:
	mov rax, [integer1]
	cmp qword [integer2], 0
	je err_divide_routine
	
	xor rdx, rdx
	mov rax,[integer1]
	idiv qword [integer2]
	jmp print_result
op_mod:
op_and:
op_or:
	mov rdi, err_op
	xor rax, rax
	call printf
	jmp end
; ERRORS
err_divide_routine:
	mov rdi, err_divide
	xor rax, rax
	call printf
	jmp end

err_op_routine:
	mov rdi, err_op
	xor rax, rax
	call printf
	jmp end

print_result:
	mov rdi, result_msg
	mov rsi, rax
	xor rax, rax
	call printf

end:
	; Cleanup
	mov rsp, rbp
	pop rbp
	xor rax, rax
	ret

