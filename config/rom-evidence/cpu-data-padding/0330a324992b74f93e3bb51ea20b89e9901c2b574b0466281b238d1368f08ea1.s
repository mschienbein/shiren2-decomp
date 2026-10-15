	.file	1 "source/src/units/main_14400/func_80093530.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_80093530
	.type	 func_80093530,@function
	.ent	func_80093530
func_80093530:
	.frame	$sp,40,$31		# vars= 8, regs= 3/0, args= 16, extra= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,40
	sw	$17,28($sp)
	move	$17,$6
	addu	$5,$5,-12
	sltu	$2,$5,28
	sw	$31,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L2
	sw	$16,24($sp)
	.set	macro
	.set	reorder

	sll	$2,$5,2
	lw	$2,.L31($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L31:
	.word	.L3
	.word	.L4
	.word	.L5
	.word	.L6
	.word	.L7
	.word	.L8
	.word	.L9
	.word	.L10
	.word	.L11
	.word	.L12
	.word	.L13
	.word	.L14
	.word	.L15
	.word	.L16
	.word	.L17
	.word	.L18
	.word	.L19
	.word	.L20
	.word	.L21
	.word	.L22
	.word	.L23
	.word	.L24
	.word	.L25
	.word	.L26
	.word	.L27
	.word	.L28
	.word	.L29
	.word	.L30
	.text
.L3:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DAE40
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L4:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DAF10
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L5:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DAFE0
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L6:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DB1F8
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L7:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,24			# 0x00000018
	.set	macro
	.set	reorder

	addu	$4,$sp,16
	la	$5,D_80141A28
	.set	noreorder
	.set	nomacro
	jal	func_8009AB34
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800DB3AC
	addu	$6,$sp,16
	.set	macro
	.set	reorder

	j	.L33
.L8:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,24			# 0x00000018
	.set	macro
	.set	reorder

	addu	$4,$sp,16
	la	$5,D_80140CF8
	.set	noreorder
	.set	nomacro
	jal	func_80097F20
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800DB7DC
	addu	$6,$sp,16
	.set	macro
	.set	reorder

	j	.L33
.L9:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DB870
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L10:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DB9A0
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L11:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DBB90
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L12:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,24			# 0x00000018
	.set	macro
	.set	reorder

	addu	$4,$sp,16
	la	$5,D_801408EC
	.set	noreorder
	.set	nomacro
	jal	func_80097F20
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800DBD00
	addu	$6,$sp,16
	.set	macro
	.set	reorder

	j	.L33
.L13:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,24			# 0x00000018
	.set	macro
	.set	reorder

	addu	$4,$sp,16
	la	$5,D_801408EC
	.set	noreorder
	.set	nomacro
	jal	func_80097F20
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800DBFF0
	addu	$6,$sp,16
	.set	macro
	.set	reorder

	j	.L33
.L14:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DC2A0
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L15:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DC3F0
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L16:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,24			# 0x00000018
	.set	macro
	.set	reorder

	addu	$4,$sp,16
	la	$5,D_801408EC
	.set	noreorder
	.set	nomacro
	jal	func_80097F20
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800DC4C0
	addu	$6,$sp,16
	.set	macro
	.set	reorder

	j	.L33
.L17:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DC730
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L18:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DC9E0
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L19:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DCB00
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L20:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DCCC0
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L21:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,20			# 0x00000014
	.set	macro
	.set	reorder

	la	$4,D_80141AD0
	.set	noreorder
	.set	nomacro
	jal	func_8009C17C
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800DCE60
	move	$6,$2
	.set	macro
	.set	reorder

	j	.L33
.L22:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,20			# 0x00000014
	.set	macro
	.set	reorder

	la	$4,D_80141994
	.set	noreorder
	.set	nomacro
	jal	func_8009E088
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800DD180
	andi	$6,$2,0x00ff
	.set	macro
	.set	reorder

	j	.L33
.L23:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,20			# 0x00000014
	.set	macro
	.set	reorder

	la	$4,D_80141AD0
	.set	noreorder
	.set	nomacro
	jal	func_8009C17C
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800DCFF0
	move	$6,$2
	.set	macro
	.set	reorder

	j	.L33
.L24:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DD320
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L25:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DD3F0
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L26:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DD4C0
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L27:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DD590
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L28:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DD6E0
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L29:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DD880
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L30:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DD970
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L33
.L2:
	move	$2,$0
.L33:
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,40
	.set	macro
	.set	reorder

	.end	func_80093530
.Lfe1:
	.size	 func_80093530,.Lfe1-func_80093530
