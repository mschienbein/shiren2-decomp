	.file	1 "source/src/units/main_14400/func_800F27A4.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_800F27A4
	.type	 func_800F27A4,@function
	.ent	func_800F27A4
func_800F27A4:
	.frame	$sp,64,$31		# vars= 16, regs= 5/0, args= 24, extra= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,64
	sw	$17,44($sp)
	move	$17,$4
	sw	$31,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$16,40($sp)
	lw	$2,0($17)
	#nop
	sw	$2,24($sp)
	lw	$2,4($17)
	move	$18,$5
	sw	$2,28($sp)
	lw	$3,0($18)
	#nop
	sltu	$2,$3,26
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L20
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lw	$2,.L131($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L131:
	.word	.L21
	.word	.L53
	.word	.L92
	.word	.L20
	.word	.L20
	.word	.L98
	.word	.L20
	.word	.L20
	.word	.L105
	.word	.L108
	.word	.L114
	.word	.L20
	.word	.L122
	.word	.L127
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L128
	.word	.L20
	.word	.L129
	.word	.L20
	.word	.L130
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L44
	.text
.L21:
	lbu	$2,D_80142F18+8
	#nop
	xori	$2,$2,0x004f
	bne	$2,$0,.L22
	jal	func_801E8DBC
	move	$16,$2
	beq	$16,$0,.L24
	la	$4,D_80147620
	jal	func_800C57A0
	andi	$2,$2,0x0003
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L24
	li	$2,41			# 0x00000029
	.set	macro
	.set	reorder

	lbu	$3,10($16)
	#nop
	bne	$3,$2,.L134
	beq	$16,$17,.L26
	j	.L24
.L134:
	lbu	$2,10($17)
	#nop
	bne	$3,$2,.L24
.L26:
	lbu	$3,84($17)
	move	$2,$0
	sw	$0,88($17)
	ori	$3,$3,0x0004
	.set	noreorder
	.set	nomacro
	j	.L133
	sb	$3,84($17)
	.set	macro
	.set	reorder

.L24:
	.set	noreorder
	.set	nomacro
	jal	func_800A50AC
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800A50E8
	move	$4,$17
	.set	macro
	.set	reorder

	j	.L133
.L22:
	.set	noreorder
	.set	nomacro
	jal	func_800F073C
	move	$4,$17
	.set	macro
	.set	reorder

	li	$3,1			# 0x00000001
	lbu	$2,9($17)
	lw	$18,88($17)
	andi	$2,$2,0x000f
	.set	noreorder
	.set	nomacro
	bne	$2,$3,.L29
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800B1C6C
	addu	$4,$sp,24
	.set	macro
	.set	reorder

	andi	$2,$2,0x0080
	sltu	$4,$0,$2
.L29:
	beq	$4,$0,.L31
	.set	noreorder
	.set	nomacro
	jal	func_800A58B8
	move	$4,$17
	.set	macro
	.set	reorder

	li	$3,2			# 0x00000002
	.set	noreorder
	.set	nomacro
	bne	$2,$3,.L133
	move	$2,$0
	.set	macro
	.set	reorder

	beq	$18,$0,.L133
	.set	noreorder
	.set	nomacro
	jal	func_800A6CF0
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$18,$2,.L133
	move	$2,$0
	.set	macro
	.set	reorder

	lbu	$2,84($17)
	#nop
	ori	$2,$2,0x0004
	.set	noreorder
	.set	nomacro
	j	.L141
	sb	$2,84($17)
	.set	macro
	.set	reorder

.L31:
	.set	noreorder
	.set	nomacro
	jal	func_800E8694
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,.L33
	lhu	$2,154($17)
	#nop
	andi	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L35
	move	$16,$0
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800E1CD4
	li	$5,16			# 0x00000010
	.set	macro
	.set	reorder

	bne	$2,$0,.L34
.L35:
	li	$16,1			# 0x00000001
.L34:
	beq	$16,$0,.L33
	.set	noreorder
	.set	nomacro
	jal	func_800E8350
	move	$4,$17
	.set	macro
	.set	reorder

	j	.L133
.L33:
	lhu	$3,154($17)
	#nop
	andi	$2,$3,0x0080
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L37
	move	$16,$0
	.set	macro
	.set	reorder

	lw	$5,D_801476B8
	.set	noreorder
	.set	nomacro
	jal	func_800A65B8
	move	$4,$17
	.set	macro
	.set	reorder

	li	$3,1			# 0x00000001
	bne	$2,$3,.L38
	addu	$16,$sp,32
	move	$4,$16
	lw	$6,D_801476B8
	.set	noreorder
	.set	nomacro
	jal	func_800A65E4
	move	$5,$17
	.set	macro
	.set	reorder

	move	$4,$17
	addu	$5,$sp,24
	.set	noreorder
	.set	nomacro
	jal	func_800A4754
	move	$6,$16
	.set	macro
	.set	reorder

	sltu	$16,$0,$2
.L38:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L39
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800F06E4
	move	$4,$17
	.set	macro
	.set	reorder

	sw	$0,88($17)
.L141:
	.set	noreorder
	.set	nomacro
	j	.L133
	move	$2,$0
	.set	macro
	.set	reorder

.L39:
	.set	noreorder
	.set	nomacro
	jal	func_800A6420
	move	$5,$18
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L40
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800F06E4
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L133
	move	$2,$0
	.set	macro
	.set	reorder

.L40:
	.set	noreorder
	.set	nomacro
	jal	func_800E776C
	li	$5,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	j	.L136
.L37:
	andi	$2,$3,0x0100
	beq	$2,$0,.L42
.L136:
	.set	noreorder
	.set	nomacro
	jal	func_800E66EC
	move	$4,$17
	.set	macro
	.set	reorder

	j	.L133
.L42:
	.set	noreorder
	.set	nomacro
	jal	func_800F0EC4
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,.L43
	.set	noreorder
	.set	nomacro
	jal	func_800F3310
	move	$4,$17
	.set	macro
	.set	reorder

	j	.L133
.L43:
	lw	$2,36($17)
	#nop
	lh	$4,160($2)
	lw	$2,164($2)
	j	.L137
.L44:
	addu	$6,$17,159
	lw	$18,4($18)
	lw	$2,36($17)
	move	$5,$18
	lh	$4,64($2)
	lw	$2,68($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$17,$4
	.set	macro
	.set	reorder

	li	$3,2			# 0x00000002
	.set	noreorder
	.set	nomacro
	beql	$2,$3,.L45
	sw	$18,88($17)
	.set	macro
	.set	reorder

.L45:
	move	$16,$0
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800F0F84
	li	$5,100			# 0x00000064
	.set	macro
	.set	reorder

	beq	$2,$0,.L46
	lhu	$2,154($17)
	#nop
	andi	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L46
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beql	$18,$0,.L46
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800B502C
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L46
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

.L46:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L53
	addu	$16,$sp,33
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800A65E4
	move	$6,$18
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800A665C
	move	$5,$16
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800E1CC4
	li	$5,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L49
	addu	$5,$sp,34
	.set	macro
	.set	reorder

	move	$4,$17
	lbu	$2,8($17)
	#nop
	addu	$2,$2,4
	andi	$2,$2,0x0007
	.set	noreorder
	.set	nomacro
	jal	func_800A665C
	sb	$2,34($sp)
	.set	macro
	.set	reorder

.L49:
	.set	noreorder
	.set	nomacro
	jal	func_800A6CF0
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,36($17)
	move	$5,$2
	lh	$4,176($3)
	lw	$2,180($3)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$17,$4
	.set	macro
	.set	reorder

	beq	$2,$0,.L50
.L142:
	.set	noreorder
	.set	nomacro
	j	.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L50:
	lw	$2,36($17)
	j	.L138
.L53:
	lbu	$2,D_80142F18+8
	#nop
	xori	$2,$2,0x004f
	bne	$2,$0,.L54
	lw	$2,D_8013960C
	la	$4,D_80147620
	sll	$2,$2,1
	sw	$2,D_8013960C
	.set	noreorder
	.set	nomacro
	jal	func_800C57A0
	move	$16,$0
	.set	macro
	.set	reorder

	andi	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L56
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800A6CF0
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,36($17)
	move	$5,$2
	lh	$4,176($3)
	lw	$2,180($3)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$17,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L56
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

.L56:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L59
	li	$4,71			# 0x00000047
	.set	macro
	.set	reorder

	move	$5,$17
	li	$6,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	move	$7,$0
	.set	macro
	.set	reorder

.L59:
	lw	$3,D_8013960C
	li	$2,1			# 0x00000001
	srl	$3,$3,$2
	sw	$3,D_8013960C
	j	.L133
.L54:
	.set	noreorder
	.set	nomacro
	jal	func_800E20CC
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L60
	sw	$0,88($17)
	.set	macro
	.set	reorder

.L60:
	lw	$18,88($17)
	#nop
	beq	$18,$0,.L61
	lw	$2,36($18)
	#nop
	lh	$4,16($2)
	lw	$2,20($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$18,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	addu	$16,$sp,35
	move	$4,$16
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800A22B8
	move	$6,$18
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800A665C
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L143
	move	$4,$17
	.set	macro
	.set	reorder

.L61:
	lbu	$2,155($17)
	#nop
	srl	$2,$2,7
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L64
	move	$16,$0
	.set	macro
	.set	reorder

	lw	$5,D_801476B8
	.set	noreorder
	.set	nomacro
	jal	func_800A65B8
	move	$4,$17
	.set	macro
	.set	reorder

	li	$3,1			# 0x00000001
	bne	$2,$3,.L64
	.set	noreorder
	.set	nomacro
	jal	func_800E20CC
	move	$4,$17
	.set	macro
	.set	reorder

	sltu	$16,$2,1
.L64:
	.set	noreorder
	.set	nomacro
	beql	$16,$0,.L63
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800F0314
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800A3B20
	move	$16,$2
	.set	macro
	.set	reorder

	move	$18,$2
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L68
	li	$4,173			# 0x000000ad
	.set	macro
	.set	reorder

	la	$4,D_80143094
	jal	func_800AFDBC
	bne	$2,$0,.L69
	.set	noreorder
	.set	nomacro
	j	.L68
	li	$4,172			# 0x000000ac
	.set	macro
	.set	reorder

.L69:
	lw	$3,D_801476B8
	#nop
	lw	$2,36($3)
	#nop
	lh	$4,152($2)
	lw	$2,156($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$3,$4
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800CD090
	move	$5,$16
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,.L72
	move	$4,$16
	.set	macro
	.set	reorder

	move	$5,$17
	lw	$6,D_801476B8
	.set	noreorder
	.set	nomacro
	jal	func_80121940
	move	$7,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L73
	addu	$3,$16,12
	.set	macro
	.set	reorder

	lhu	$2,154($17)
	#nop
	andi	$2,$2,0xff7f
	.set	noreorder
	.set	nomacro
	j	.L74
	sh	$2,154($17)
	.set	macro
	.set	reorder

.L73:
	lw	$2,4($3)
	#nop
	lh	$4,16($2)
	lw	$2,20($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$3,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L74
	li	$4,171			# 0x000000ab
	.set	macro
	.set	reorder

	j	.L68
.L72:
	li	$4,170			# 0x000000aa
.L68:
	lhu	$2,154($17)
	move	$5,$18
	andi	$2,$2,0xff7f
	.set	noreorder
	.set	nomacro
	jal	func_800498E4
	sh	$2,154($17)
	.set	macro
	.set	reorder

.L74:
	lbu	$2,155($17)
	#nop
	srl	$2,$2,7
	xori	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L142
	move	$4,$0
	.set	macro
	.set	reorder

	li	$5,-1			# 0xffffffff
	.set	noreorder
	.set	nomacro
	jal	func_800A08D8
	move	$6,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L63:
.L143:
	.set	noreorder
	.set	nomacro
	jal	func_800E1CC4
	li	$5,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L80
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$2,8($17)
	addu	$5,$sp,36
	addu	$2,$2,4
	andi	$2,$2,0x0007
	.set	noreorder
	.set	nomacro
	jal	func_800A665C
	sb	$2,36($sp)
	.set	macro
	.set	reorder

.L80:
	.set	noreorder
	.set	nomacro
	jal	func_800A6CF0
	move	$4,$17
	.set	macro
	.set	reorder

	move	$16,$0
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800F0EC4
	move	$19,$2
	.set	macro
	.set	reorder

	bne	$2,$0,.L81
	.set	noreorder
	.set	nomacro
	jal	func_800E20CC
	move	$4,$17
	.set	macro
	.set	reorder

	sltu	$16,$2,1
.L81:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L145
	move	$16,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$18,$0,.L84
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800A6420
	move	$5,$18
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	beq	$2,$0,.L85
	lhu	$2,154($17)
	#nop
	andi	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L83
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

.L85:
	lbu	$5,136($17)
	la	$4,D_80147620
	jal	func_800C587C
	beq	$2,$0,.L83
	.set	noreorder
	.set	nomacro
	beql	$18,$19,.L83
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	lhu	$2,154($17)
	#nop
	andi	$2,$2,0x0001
	beq	$2,$0,.L83
.L84:
	li	$16,1			# 0x00000001
.L83:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L82
	move	$5,$19
	.set	macro
	.set	reorder

	lw	$2,36($17)
	#nop
	lh	$4,176($2)
	lw	$2,180($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$17,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L145
	move	$16,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L142
	sb	$0,118($17)
	.set	macro
	.set	reorder

.L82:
	move	$16,$0
.L145:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800E1CC4
	li	$5,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L88
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800E1D14
	li	$5,19			# 0x00000013
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L88
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800E1CC4
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L88
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800E20CC
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L88
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L88
	move	$4,$17
	.set	macro
	.set	reorder

	addu	$5,$sp,24
	.set	noreorder
	.set	nomacro
	jal	func_800A46BC
	addu	$6,$17,8
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L88
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800A44F4
	move	$5,$19
	.set	macro
	.set	reorder

	li	$3,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	bnel	$2,$3,.L88
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

.L88:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L133
	move	$2,$0
	.set	macro
	.set	reorder

	lw	$2,36($17)
	sb	$0,118($17)
.L138:
	lh	$4,168($2)
	lw	$2,172($2)
.L137:
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$17,$4
	.set	macro
	.set	reorder

	j	.L133
.L92:
	lbu	$2,114($17)
	#nop
	andi	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L146
	move	$16,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800F0A08
	move	$4,$17
	.set	macro
	.set	reorder

	move	$16,$0
.L146:
	.set	noreorder
	.set	nomacro
	jal	func_800E1D54
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,.L94
	lbu	$2,9($17)
	#nop
	andi	$2,$2,0x000f
	xori	$2,$2,0x0002
	sltu	$16,$2,1
.L94:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L20
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$2,36($17)
	li	$6,13			# 0x0000000d
	lh	$4,144($2)
	move	$7,$0
	sw	$0,16($sp)
	lw	$2,148($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$17,$4
	.set	macro
	.set	reorder

	move	$5,$0
	lw	$2,36($17)
	li	$6,13			# 0x0000000d
	lh	$4,144($2)
	li	$7,254			# 0x000000fe
	sw	$0,16($sp)
	lw	$2,148($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$17,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L144
	move	$4,$17
	.set	macro
	.set	reorder

.L98:
	lw	$2,16($18)
	#nop
	addu	$2,$2,-4
	sltu	$2,$2,2
	beq	$2,$0,.L99
	.set	noreorder
	.set	nomacro
	jal	func_800F0314
	move	$4,$17
	.set	macro
	.set	reorder

	move	$16,$2
	beq	$16,$0,.L99
	lw	$3,D_801476B8
	#nop
	lw	$2,36($3)
	#nop
	lh	$4,152($2)
	lw	$2,156($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$3,$4
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800CD090
	move	$5,$16
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffff
	beq	$2,$3,.L99
	.set	noreorder
	.set	nomacro
	jal	func_800F0404
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L144
	move	$4,$17
	.set	macro
	.set	reorder

.L99:
	lhu	$4,154($17)
	#nop
	andi	$2,$4,0x0040
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L102
	move	$3,$0
	.set	macro
	.set	reorder

	andi	$2,$4,0x0200
	sltu	$3,$0,$2
.L102:
	.set	noreorder
	.set	nomacro
	beq	$3,$0,.L20
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$2,157($17)
	#nop
	srl	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_800D78B4
	sb	$2,157($17)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L144
	move	$4,$17
	.set	macro
	.set	reorder

.L105:
	lbu	$2,D_80142F18+8
	#nop
	xori	$2,$2,0x004f
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$2,16($18)
	#nop
	lw	$5,0($2)
	.set	noreorder
	.set	nomacro
	jal	func_800F0CB0
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L144
	move	$4,$17
	.set	macro
	.set	reorder

.L108:
	lbu	$2,D_80142F18+8
	#nop
	xori	$2,$2,0x004f
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$16,16($18)
	#nop
	lw	$5,0($16)
	.set	noreorder
	.set	nomacro
	jal	func_800F0CB0
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800F3408
	move	$5,$18
	.set	macro
	.set	reorder

	lhu	$3,154($17)
	#nop
	andi	$2,$3,0x0008
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L111
	move	$18,$0
	.set	macro
	.set	reorder

	andi	$2,$3,0x0040
	bne	$2,$0,.L111
	.set	noreorder
	.set	nomacro
	jal	func_800E08B0
	move	$4,$17
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	beq	$2,$0,.L111
	lbu	$5,158($17)
	la	$4,D_80147620
	jal	func_800C587C
	sltu	$18,$0,$2
.L111:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	lh	$2,12($16)
	#nop
	.set	noreorder
	.set	nomacro
	blez	$2,.L142
	li	$2,11			# 0x0000000b
	.set	macro
	.set	reorder

	lw	$5,4($16)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$5,$2,.L142
	li	$2,31			# 0x0000001f
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$2,.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800F0C4C
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L114:
	lbu	$2,D_80142F18+8
	#nop
	xori	$2,$2,0x004f
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L142
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$16,16($18)
	.set	noreorder
	.set	nomacro
	jal	func_800E3D20
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L117
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,4($16)
	#nop
	xori	$5,$5,0x0027
	.set	noreorder
	.set	nomacro
	jal	func_800F16C0
	sltu	$5,$5,1
	.set	macro
	.set	reorder

.L117:
	lw	$3,4($16)
	li	$2,30			# 0x0000001e
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L147
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$2,D_8013960C
	#nop
	sll	$2,$2,1
	ori	$2,$2,0x0001
	sw	$2,D_8013960C
	.set	noreorder
	.set	nomacro
	jal	func_800F0440
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$2,D_8013960C
	lhu	$3,154($17)
	srl	$2,$2,1
	sw	$2,D_8013960C
	andi	$2,$3,0x0040
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L119
	move	$4,$0
	.set	macro
	.set	reorder

	andi	$2,$3,0x0200
	sltu	$4,$0,$2
.L119:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,.L118
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$2,157($17)
	#nop
	srl	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_800D78B4
	sb	$2,157($17)
	.set	macro
	.set	reorder

.L118:
	move	$4,$17
.L147:
	.set	noreorder
	.set	nomacro
	jal	func_800E42AC
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L133
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L122:
	lw	$4,140($17)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$4,$0,.L124
	move	$16,$0
	.set	macro
	.set	reorder

	jal	func_800CF47C
	bne	$2,$0,.L123
.L124:
	li	$16,1			# 0x00000001
.L123:
	beq	$16,$0,.L125
	lbu	$2,114($17)
	#nop
	andi	$2,$2,0x0008
	beq	$2,$0,.L125
	.set	noreorder
	.set	nomacro
	jal	func_800AACE0
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L125
	move	$4,$2
	.set	macro
	.set	reorder

	move	$5,$17
	lbu	$2,2($4)
	move	$6,$17
	ori	$2,$2,0x0040
	.set	noreorder
	.set	nomacro
	jal	func_800ADC90
	sb	$2,2($4)
	.set	macro
	.set	reorder

.L125:
	lbu	$3,114($17)
	li	$2,1			# 0x00000001
	andi	$3,$3,0x00f7
	.set	noreorder
	.set	nomacro
	j	.L133
	sb	$3,114($17)
	.set	macro
	.set	reorder

.L127:
	lw	$5,4($18)
	.set	noreorder
	.set	nomacro
	jal	func_800F3718
	move	$4,$17
	.set	macro
	.set	reorder

	j	.L133
.L128:
	li	$2,1			# 0x00000001
	lw	$4,4($18)
	li	$3,10			# 0x0000000a
	sb	$3,159($17)
	.set	noreorder
	.set	nomacro
	j	.L133
	sw	$4,88($17)
	.set	macro
	.set	reorder

.L129:
	lw	$5,4($18)
	.set	noreorder
	.set	nomacro
	jal	func_800F0CB0
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	lw	$2,20($18)
	lw	$5,4($18)
	lbu	$7,0($2)
	lw	$6,16($18)
	.set	noreorder
	.set	nomacro
	jal	func_800F3538
	sll	$7,$7,24
	.set	macro
	.set	reorder

	j	.L133
.L130:
	lbu	$5,19($18)
	lbu	$6,23($18)
	.set	noreorder
	.set	nomacro
	jal	func_800F38AC
	move	$4,$17
	.set	macro
	.set	reorder

	j	.L133
.L20:
	move	$4,$17
.L144:
	.set	noreorder
	.set	nomacro
	jal	func_800E4B60
	move	$5,$18
	.set	macro
	.set	reorder

.L133:
	lw	$31,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,64
	.set	macro
	.set	reorder

	.end	func_800F27A4
.Lfe1:
	.size	 func_800F27A4,.Lfe1-func_800F27A4
