	.file	"mutex.c"
	.text
.Ltext0:
	.globl	main
	.type	main, @function
main:
.LFB2:
	.file 1 "mutex.c"
	.loc 1 4 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.loc 1 4 0
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	.loc 1 5 0
	movq	$0, -48(%rbp)
	movq	$0, -40(%rbp)
	movq	$0, -32(%rbp)
	movq	$0, -24(%rbp)
	movq	$0, -16(%rbp)
	.loc 1 6 0
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	pthread_mutex_lock
	.loc 1 7 0
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L2
	call	__stack_chk_fail
.L2:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
.Letext0:
	.file 2 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x18e
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF28
	.byte	0xc
	.long	.LASF29
	.long	.LASF30
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF0
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF1
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF7
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF8
	.uleb128 0x4
	.long	.LASF11
	.byte	0x10
	.byte	0x2
	.byte	0x4b
	.long	0x98
	.uleb128 0x5
	.long	.LASF9
	.byte	0x2
	.byte	0x4d
	.long	0x98
	.byte	0
	.uleb128 0x5
	.long	.LASF10
	.byte	0x2
	.byte	0x4e
	.long	0x98
	.byte	0x8
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0x73
	.uleb128 0x7
	.long	.LASF24
	.byte	0x2
	.byte	0x4f
	.long	0x73
	.uleb128 0x4
	.long	.LASF12
	.byte	0x28
	.byte	0x2
	.byte	0x5c
	.long	0x116
	.uleb128 0x5
	.long	.LASF13
	.byte	0x2
	.byte	0x5e
	.long	0x57
	.byte	0
	.uleb128 0x5
	.long	.LASF14
	.byte	0x2
	.byte	0x5f
	.long	0x3b
	.byte	0x4
	.uleb128 0x5
	.long	.LASF15
	.byte	0x2
	.byte	0x60
	.long	0x57
	.byte	0x8
	.uleb128 0x5
	.long	.LASF16
	.byte	0x2
	.byte	0x62
	.long	0x3b
	.byte	0xc
	.uleb128 0x5
	.long	.LASF17
	.byte	0x2
	.byte	0x66
	.long	0x57
	.byte	0x10
	.uleb128 0x5
	.long	.LASF18
	.byte	0x2
	.byte	0x68
	.long	0x50
	.byte	0x14
	.uleb128 0x5
	.long	.LASF19
	.byte	0x2
	.byte	0x69
	.long	0x50
	.byte	0x16
	.uleb128 0x5
	.long	.LASF20
	.byte	0x2
	.byte	0x6a
	.long	0x9e
	.byte	0x18
	.byte	0
	.uleb128 0x8
	.byte	0x28
	.byte	0x2
	.byte	0x5a
	.long	0x140
	.uleb128 0x9
	.long	.LASF21
	.byte	0x2
	.byte	0x7d
	.long	0xa9
	.uleb128 0x9
	.long	.LASF22
	.byte	0x2
	.byte	0x7e
	.long	0x140
	.uleb128 0x9
	.long	.LASF23
	.byte	0x2
	.byte	0x7f
	.long	0x5e
	.byte	0
	.uleb128 0xa
	.long	0x6c
	.long	0x150
	.uleb128 0xb
	.long	0x65
	.byte	0x27
	.byte	0
	.uleb128 0x7
	.long	.LASF25
	.byte	0x2
	.byte	0x80
	.long	0x116
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF26
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF27
	.uleb128 0xc
	.long	.LASF31
	.byte	0x1
	.byte	0x3
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0xd
	.long	.LASF32
	.byte	0x1
	.byte	0x5
	.long	0x150
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x17
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF32:
	.string	"mutex"
.LASF29:
	.string	"mutex.c"
.LASF21:
	.string	"__data"
.LASF30:
	.string	"/home/manolis/Desktop/OS/EX/Assembly"
.LASF12:
	.string	"__pthread_mutex_s"
.LASF15:
	.string	"__owner"
.LASF17:
	.string	"__kind"
.LASF25:
	.string	"pthread_mutex_t"
.LASF22:
	.string	"__size"
.LASF0:
	.string	"unsigned char"
.LASF3:
	.string	"long unsigned int"
.LASF1:
	.string	"short unsigned int"
.LASF11:
	.string	"__pthread_internal_list"
.LASF19:
	.string	"__elision"
.LASF31:
	.string	"main"
.LASF28:
	.string	"GNU C11 5.4.0 20160609 -mtune=generic -march=x86-64 -g -fstack-protector-strong"
.LASF2:
	.string	"unsigned int"
.LASF26:
	.string	"long long unsigned int"
.LASF18:
	.string	"__spins"
.LASF7:
	.string	"sizetype"
.LASF27:
	.string	"long long int"
.LASF8:
	.string	"char"
.LASF23:
	.string	"__align"
.LASF16:
	.string	"__nusers"
.LASF14:
	.string	"__count"
.LASF13:
	.string	"__lock"
.LASF5:
	.string	"short int"
.LASF9:
	.string	"__prev"
.LASF24:
	.string	"__pthread_list_t"
.LASF6:
	.string	"long int"
.LASF20:
	.string	"__list"
.LASF10:
	.string	"__next"
.LASF4:
	.string	"signed char"
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.5) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
