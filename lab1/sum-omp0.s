	.file	"sum-omp.c"
	.globl	sum
	.bss
	.align 4
	.type	sum, @object
	.size	sum, 4
sum:
	.zero	4
	.text
	.globl	compute
	.type	compute, @function
compute:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	imull	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	compute, .-compute
	.section	.rodata
	.align 8
.LC0:
	.string	"The sum of 1,2,...,1000 is %d (should be 332833500)\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L4
.L5:
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	compute
	movl	%eax, %edx
	movl	sum(%rip), %eax
	addl	%edx, %eax
	movl	%eax, sum(%rip)
	addl	$1, -4(%rbp)
.L4:
	cmpl	$999, -4(%rbp)
	jle	.L5
	movl	sum(%rip), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.5) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
