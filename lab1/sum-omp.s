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
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$0, %ecx
	movl	$0, %edx
	movl	$0, %esi
	movl	$main._omp_fn.0, %edi
	call	GOMP_parallel
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
	.type	main._omp_fn.0, @function
main._omp_fn.0:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movl	$0, -24(%rbp)
	call	omp_get_num_threads
	movl	%eax, %ebx
	call	omp_get_thread_num
	movl	%eax, %esi
	movl	$1000, %eax
	cltd
	idivl	%ebx
	movl	%eax, %ecx
	movl	$1000, %eax
	cltd
	idivl	%ebx
	movl	%edx, %eax
	cmpl	%eax, %esi
	jl	.L6
.L9:
	imull	%ecx, %esi
	movl	%esi, %edx
	addl	%edx, %eax
	leal	(%rax,%rcx), %ebx
	cmpl	%ebx, %eax
	jge	.L7
	movl	%eax, -20(%rbp)
.L8:
	movl	-20(%rbp), %eax
	movl	%eax, %edi
	call	compute
	movl	%eax, %edx
	movl	-24(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -24(%rbp)
	addl	$1, -20(%rbp)
	cmpl	%ebx, -20(%rbp)
	jl	.L8
.L7:
	movl	-24(%rbp), %eax
	lock addl	%eax, sum(%rip)
	jmp	.L10
.L6:
	movl	$0, %eax
	addl	$1, %ecx
	jmp	.L9
.L10:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main._omp_fn.0, .-main._omp_fn.0
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.5) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
