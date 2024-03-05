	.file	"fork.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"I am the child! My PID is %d.\n"
	.text
	.type	child, @function
child:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	getpid@PLT
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
.LFE6:
	.size	child, .-child
	.section	.rodata
	.align 8
.LC1:
	.string	"I am the parent! My PID is %d and the PID of my child is %d.\n"
	.text
	.type	parent, @function
parent:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	call	getpid@PLT
	movl	%eax, %ecx
	movl	-4(%rbp), %eax
	movl	%eax, %edx
	movl	%ecx, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
.LFE7:
	.size	parent, .-parent
	.section	.rodata
.LC2:
	.string	"fork() Faliure"
	.align 8
.LC3:
	.string	"\354\235\264\352\263\263\354\227\220\353\212\224 \353\217\204\353\213\254\355\225\230\354\247\200 \354\225\212\354\225\204\354\225\274 \355\225\251\353\213\210\353\213\244."
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	call	fork@PLT
	movl	%eax, -4(%rbp)
	cmpl	$-1, -4(%rbp)
	jne	.L4
	leaq	.LC2(%rip), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	err@PLT
.L4:
	cmpl	$0, -4(%rbp)
	jne	.L5
	movl	$0, %eax
	call	child
	jmp	.L6
.L5:
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	parent
.L6:
	leaq	.LC3(%rip), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	err@PLT
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
