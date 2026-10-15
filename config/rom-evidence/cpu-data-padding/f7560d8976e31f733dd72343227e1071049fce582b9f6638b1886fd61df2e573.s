	.file	1 "source/src/units/main_14400/func_8004FA90.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_8004FA90
	.type	 func_8004FA90,@function
	.ent	func_8004FA90
func_8004FA90:
	.frame	$sp,56,$31		# vars= 8, regs= 4/0, args= 32, extra= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,56
	sw	$18,48($sp)
	move	$18,$4
	sw	$31,52($sp)
	sw	$17,44($sp)
	.set	noreorder
	.set	nomacro
	bne	$18,$0,.L2
	sw	$16,40($sp)
	.set	macro
	.set	reorder

	sw	$0,36($sp)
	.set	noreorder
	.set	nomacro
	j	.L3
	sw	$0,32($sp)
	.set	macro
	.set	reorder

.L2:
	lw	$2,4($18)
	#nop
	sw	$2,32($sp)
	lw	$2,0($18)
	#nop
	sw	$2,36($sp)
.L3:
	lw	$2,D_8013968C
	#nop
	addu	$3,$2,-228
	sltu	$2,$3,64
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L1
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lw	$2,.L57($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L57:
	.word	.L5
	.word	.L1
	.word	.L8
	.word	.L9
	.word	.L10
	.word	.L12
	.word	.L13
	.word	.L19
	.word	.L15
	.word	.L1
	.word	.L1
	.word	.L1
	.word	.L20
	.word	.L21
	.word	.L22
	.word	.L23
	.word	.L24
	.word	.L25
	.word	.L26
	.word	.L1
	.word	.L1
	.word	.L1
	.word	.L1
	.word	.L31
	.word	.L1
	.word	.L27
	.word	.L28
	.word	.L29
	.word	.L1
	.word	.L1
	.word	.L30
	.word	.L1
	.word	.L1
	.word	.L1
	.word	.L1
	.word	.L32
	.word	.L34
	.word	.L35
	.word	.L36
	.word	.L38
	.word	.L1
	.word	.L1
	.word	.L1
	.word	.L39
	.word	.L1
	.word	.L1
	.word	.L1
	.word	.L40
	.word	.L41
	.word	.L42
	.word	.L43
	.word	.L44
	.word	.L45
	.word	.L46
	.word	.L1
	.word	.L48
	.word	.L49
	.word	.L50
	.word	.L1
	.word	.L1
	.word	.L51
	.word	.L52
	.word	.L54
	.word	.L53
	.text
.L5:
	.set	noreorder
	.set	nomacro
	jal	func_800B4D80
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80084A68
	move	$16,$2
	.set	macro
	.set	reorder

	jal	func_80084BCC
	andi	$4,$2,0xffff
	li	$2,1			# 0x00000001
	sw	$2,16($sp)
	lw	$6,32($sp)
	lw	$7,36($sp)
	la	$5,func_800887F0
	jal	func_80084C00
	bltz	$2,.L6
	la	$4,func_8008865C
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,4			# 0x00000004
	.set	macro
	.set	reorder

.L6:
	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	li	$4,20			# 0x00000014
	.set	macro
	.set	reorder

	la	$4,func_8008865C
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,6			# 0x00000006
	.set	macro
	.set	reorder

	beq	$16,$0,.L1
	lbu	$2,2($16)
	#nop
	andi	$2,$2,0x0010
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L1
	move	$5,$0
	.set	macro
	.set	reorder

	la	$4,func_8008B558
	j	.L59
.L8:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,431			# 0x000001af
	.set	macro
	.set	reorder

.L9:
	jal	func_80084A20
	li	$4,458			# 0x000001ca
	move	$5,$18
	li	$6,6			# 0x00000006
	.set	noreorder
	.set	nomacro
	jal	func_80050F34
	move	$7,$0
	.set	macro
	.set	reorder

	jal	func_80084B80
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,457			# 0x000001c9
	.set	macro
	.set	reorder

.L10:
	.set	noreorder
	.set	nomacro
	jal	func_800B4928
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L60
	li	$4,453			# 0x000001c5
	.set	macro
	.set	reorder

	jal	func_80084A20
	li	$4,454			# 0x000001c6
	.set	noreorder
	.set	nomacro
	jal	func_80050E44
	move	$5,$18
	.set	macro
	.set	reorder

	jal	func_80084B80
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,453			# 0x000001c5
	.set	macro
	.set	reorder

.L12:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,459			# 0x000001cb
	.set	macro
	.set	reorder

.L13:
	.set	noreorder
	.set	nomacro
	jal	func_800B4D80
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L60
	li	$4,438			# 0x000001b6
	.set	macro
	.set	reorder

	lbu	$3,1($2)
	li	$2,219			# 0x000000db
	.set	noreorder
	.set	nomacro
	bnel	$3,$2,.L60
	li	$4,438			# 0x000001b6
	.set	macro
	.set	reorder

	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	lw	$6,36($sp)
	li	$5,780861440			# 0x2e8b0000
	mult	$6,$2
	ori	$5,$5,0xa2e9
	la	$4,func_80085530
	lw	$7,32($sp)
	sra	$2,$6,31
	mfhi	$8
	#nop
	#nop
	sra	$3,$8,2
	subu	$3,$3,$2
	sll	$2,$3,2
	addu	$2,$2,$3
	sll	$2,$2,1
	subu	$6,$6,$2
	mult	$7,$5
	sll	$5,$6,1
	addu	$5,$5,$6
	sll	$5,$5,2
	subu	$5,$5,$6
	sra	$2,$7,31
	mfhi	$8
	#nop
	#nop
	sra	$3,$8,1
	subu	$3,$3,$2
	sll	$2,$3,1
	addu	$2,$2,$3
	sll	$2,$2,2
	subu	$2,$2,$3
	subu	$7,$7,$2
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	addu	$5,$5,$7
	.set	macro
	.set	reorder

	lw	$3,32($sp)
	#nop
	sw	$3,92($2)
	lw	$4,36($sp)
	li	$3,5			# 0x00000005
	sw	$3,28($2)
	sw	$4,104($2)
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,438			# 0x000001b6
	.set	macro
	.set	reorder

.L15:
	jal	func_800C5F60
	.set	noreorder
	.set	nomacro
	jal	func_800A8C00
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80084A68
	andi	$16,$2,0x00ff
	.set	macro
	.set	reorder

	la	$4,func_8008AA20
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	move	$5,$16
	.set	macro
	.set	reorder

	lw	$3,32($sp)
	move	$17,$2
	sw	$3,92($17)
	lw	$2,36($sp)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	func_800B4928
	sw	$2,104($17)
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L16
	li	$2,-1			# 0xffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800A8C00
	move	$4,$16
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	sw	$2,36($17)
	lbu	$3,31($16)
	li	$2,23			# 0x00000017
	.set	noreorder
	.set	nomacro
	bnel	$3,$2,.L63
	li	$4,446			# 0x000001be
	.set	macro
	.set	reorder

	lhu	$2,18($17)
	#nop
	ori	$2,$2,0x4000
	.set	noreorder
	.set	nomacro
	j	.L18
	sh	$2,18($17)
	.set	macro
	.set	reorder

.L16:
	sw	$2,36($17)
.L18:
	li	$4,446			# 0x000001be
.L63:
	.set	noreorder
	.set	nomacro
	jal	func_80050E44
	move	$5,$18
	.set	macro
	.set	reorder

	jal	func_80084A20
	li	$4,448			# 0x000001c0
	.set	noreorder
	.set	nomacro
	jal	func_80050E44
	move	$5,$18
	.set	macro
	.set	reorder

	jal	func_80084B80
	jal	func_80084A20
	la	$16,func_8008865C
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,2			# 0x00000002
	.set	macro
	.set	reorder

	li	$4,448			# 0x000001c0
	.set	noreorder
	.set	nomacro
	jal	func_80050E44
	move	$5,$18
	.set	macro
	.set	reorder

	jal	func_80084B80
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,447			# 0x000001bf
	.set	macro
	.set	reorder

.L19:
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	lw	$6,36($sp)
	li	$5,780861440			# 0x2e8b0000
	mult	$6,$2
	ori	$5,$5,0xa2e9
	la	$4,func_80085530
	lw	$7,32($sp)
	sra	$2,$6,31
	mfhi	$8
	#nop
	#nop
	sra	$3,$8,2
	subu	$3,$3,$2
	sll	$2,$3,2
	addu	$2,$2,$3
	sll	$2,$2,1
	subu	$6,$6,$2
	mult	$7,$5
	sll	$5,$6,1
	addu	$5,$5,$6
	sll	$5,$5,2
	subu	$5,$5,$6
	sra	$2,$7,31
	mfhi	$8
	#nop
	#nop
	sra	$3,$8,1
	subu	$3,$3,$2
	sll	$2,$3,1
	addu	$2,$2,$3
	sll	$2,$2,2
	subu	$2,$2,$3
	subu	$7,$7,$2
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	addu	$5,$5,$7
	.set	macro
	.set	reorder

	li	$4,461			# 0x000001cd
	lw	$3,32($sp)
	move	$5,$18
	sw	$3,92($2)
	lw	$6,36($sp)
	li	$3,25			# 0x00000019
	sw	$3,28($2)
	.set	noreorder
	.set	nomacro
	j	.L61
	sw	$6,104($2)
	.set	macro
	.set	reorder

.L20:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,455			# 0x000001c7
	.set	macro
	.set	reorder

.L21:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,463			# 0x000001cf
	.set	macro
	.set	reorder

.L22:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,462			# 0x000001ce
	.set	macro
	.set	reorder

.L23:
	jal	func_80084A20
	li	$4,452			# 0x000001c4
	move	$5,$18
	li	$6,6			# 0x00000006
	.set	noreorder
	.set	nomacro
	jal	func_80050F34
	move	$7,$0
	.set	macro
	.set	reorder

	jal	func_80084B80
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,451			# 0x000001c3
	.set	macro
	.set	reorder

.L24:
	jal	func_80084A20
	li	$4,442			# 0x000001ba
	move	$5,$18
	li	$6,6			# 0x00000006
	.set	noreorder
	.set	nomacro
	jal	func_80050F34
	move	$7,$0
	.set	macro
	.set	reorder

	jal	func_80084B80
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,441			# 0x000001b9
	.set	macro
	.set	reorder

.L25:
	.set	noreorder
	.set	nomacro
	j	.L62
	li	$4,449			# 0x000001c1
	.set	macro
	.set	reorder

.L26:
	li	$4,440			# 0x000001b8
.L62:
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	func_80050B3C
	li	$6,5			# 0x00000005
	.set	macro
	.set	reorder

	j	.L1
.L27:
	la	$16,func_8008865C
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,7			# 0x00000007
	.set	macro
	.set	reorder

	jal	func_80084A20
	li	$4,322			# 0x00000142
	.set	noreorder
	.set	nomacro
	jal	func_80050E44
	move	$5,$18
	.set	macro
	.set	reorder

	jal	func_80084B80
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,24			# 0x00000018
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,320			# 0x00000140
	.set	macro
	.set	reorder

.L28:
	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	li	$4,26			# 0x0000001a
	.set	macro
	.set	reorder

	move	$4,$0
	lw	$6,32($sp)
	lw	$7,36($sp)
	.set	noreorder
	.set	nomacro
	jal	func_8007920C
	li	$5,294			# 0x00000126
	.set	macro
	.set	reorder

	li	$4,4			# 0x00000004
	move	$17,$2
	.set	noreorder
	.set	nomacro
	jal	func_8007946C
	move	$5,$17
	.set	macro
	.set	reorder

	li	$4,4			# 0x00000004
	move	$5,$17
	li	$6,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_80079560
	move	$16,$2
	.set	macro
	.set	reorder

	la	$4,func_8008B004
	li.s	$f0,5.00000000000000000000e-1
	li.s	$f2,5.00000000000000000000e0
	move	$5,$17
	s.s	$f0,32($16)
	s.s	$f0,28($16)
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	s.s	$f2,20($16)
	.set	macro
	.set	reorder

	li	$3,4			# 0x00000004
	sw	$3,44($2)
	la	$3,D_8013F150
	.set	noreorder
	.set	nomacro
	j	.L1
	sw	$3,32($2)
	.set	macro
	.set	reorder

.L29:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,328			# 0x00000148
	.set	macro
	.set	reorder

.L30:
	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	li	$4,175			# 0x000000af
	.set	macro
	.set	reorder

	j	.L1
.L31:
	la	$16,func_8008865C
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,7			# 0x00000007
	.set	macro
	.set	reorder

	jal	func_80084A20
	li	$4,322			# 0x00000142
	.set	noreorder
	.set	nomacro
	jal	func_80050E44
	move	$5,$18
	.set	macro
	.set	reorder

	jal	func_80084B80
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,23			# 0x00000017
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	li	$4,125			# 0x0000007d
	.set	macro
	.set	reorder

	j	.L1
.L32:
	.set	noreorder
	.set	nomacro
	jal	func_800B4D80
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L1
	move	$5,$0
	.set	macro
	.set	reorder

	la	$4,func_8008A7B0
	j	.L59
.L34:
	la	$4,func_8008865C
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	j	.L1
.L35:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,431			# 0x000001af
	.set	macro
	.set	reorder

.L36:
	jal	func_80048AE4
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L60
	li	$4,309			# 0x00000135
	.set	macro
	.set	reorder

	la	$4,func_800869E8
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,309			# 0x00000135
	.set	macro
	.set	reorder

.L38:
	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	li	$4,33			# 0x00000021
	.set	macro
	.set	reorder

	la	$4,func_8008AEC8
	li	$5,-1			# 0xffffffff
.L59:
	jal	func_80085154
	lw	$3,32($sp)
	#nop
	sw	$3,92($2)
	lw	$3,36($sp)
	.set	noreorder
	.set	nomacro
	j	.L1
	sw	$3,104($2)
	.set	macro
	.set	reorder

.L39:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,308			# 0x00000134
	.set	macro
	.set	reorder

.L40:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,321			# 0x00000141
	.set	macro
	.set	reorder

.L41:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,157			# 0x0000009d
	.set	macro
	.set	reorder

.L42:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,56			# 0x00000038
	.set	macro
	.set	reorder

.L43:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,52			# 0x00000034
	.set	macro
	.set	reorder

.L44:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,148			# 0x00000094
	.set	macro
	.set	reorder

.L45:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,164			# 0x000000a4
	.set	macro
	.set	reorder

.L46:
	move	$4,$0
	lw	$6,32($sp)
	lw	$7,36($sp)
	.set	noreorder
	.set	nomacro
	jal	func_8007920C
	li	$5,385			# 0x00000181
	.set	macro
	.set	reorder

	move	$17,$2
	.set	noreorder
	.set	nomacro
	bltz	$17,.L1
	li	$4,4			# 0x00000004
	.set	macro
	.set	reorder

	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_80079560
	li	$6,1			# 0x00000001
	.set	macro
	.set	reorder

	la	$16,func_8008B004
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	move	$5,$17
	.set	macro
	.set	reorder

	move	$3,$2
	move	$4,$16
	move	$5,$17
	li	$16,4			# 0x00000004
	la	$2,D_8013F178
	sw	$16,44($3)
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	sw	$2,32($3)
	.set	macro
	.set	reorder

	move	$3,$2
	la	$2,D_8013F1D0
	sw	$16,44($3)
	.set	noreorder
	.set	nomacro
	j	.L1
	sw	$2,32($3)
	.set	macro
	.set	reorder

.L48:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,164			# 0x000000a4
	.set	macro
	.set	reorder

.L49:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,169			# 0x000000a9
	.set	macro
	.set	reorder

.L50:
	la	$4,func_8008865C
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,322			# 0x00000142
	.set	macro
	.set	reorder

.L51:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,432			# 0x000001b0
	.set	macro
	.set	reorder

.L52:
	li	$4,456			# 0x000001c8
	move	$5,$18
	li	$6,2			# 0x00000002
	move	$7,$0
	li	$2,44			# 0x0000002c
	sw	$0,16($sp)
	sw	$0,20($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80050EB4
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	j	.L1
.L53:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,445			# 0x000001bd
	.set	macro
	.set	reorder

.L54:
	jal	func_800A99D0
	beq	$2,$0,.L55
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$4,431			# 0x000001af
	.set	macro
	.set	reorder

.L55:
	jal	func_80084A20
	la	$17,func_800869B8
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	li	$5,-1			# 0xffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	li	$4,424			# 0x000001a8
	.set	macro
	.set	reorder

	la	$16,func_8008865C
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,15			# 0x0000000f
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	li	$4,198			# 0x000000c6
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,32			# 0x00000020
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	li	$4,169			# 0x000000a9
	.set	macro
	.set	reorder

	lw	$2,D_801476B8
	#nop
	lw	$5,132($2)
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	move	$4,$17
	.set	macro
	.set	reorder

	jal	func_80084B80
	li	$4,437			# 0x000001b5
.L60:
	move	$5,$18
.L61:
	jal	func_80050E44
.L1:
	lw	$31,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,56
	.set	macro
	.set	reorder

	.end	func_8004FA90
.Lfe1:
	.size	 func_8004FA90,.Lfe1-func_8004FA90
