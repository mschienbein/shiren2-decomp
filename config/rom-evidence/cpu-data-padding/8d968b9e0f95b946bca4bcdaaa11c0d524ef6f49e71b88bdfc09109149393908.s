	.file	1 "source/src/units/main_14400/func_8004D588.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_8004D588
	.type	 func_8004D588,@function
	.ent	func_8004D588
func_8004D588:
	.frame	$sp,112,$31		# vars= 48, regs= 10/0, args= 24, extra= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,112
	sw	$20,88($sp)
	move	$20,$4
	sw	$21,92($sp)
	move	$21,$5
	sw	$22,96($sp)
	move	$22,$6
	sw	$31,108($sp)
	sw	$fp,104($sp)
	sw	$23,100($sp)
	sw	$19,84($sp)
	sw	$18,80($sp)
	sw	$17,76($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800A8C00
	sw	$16,72($sp)
	.set	macro
	.set	reorder

	lw	$3,0($20)
	#nop
	sw	$3,24($sp)
	lw	$3,4($20)
	addu	$16,$sp,24
	sw	$3,4($16)
	lw	$3,D_80139694
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$0,.L12
	andi	$23,$2,0x00ff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80048B04
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L12
	li	$2,71			# 0x00000047
	.set	macro
	.set	reorder

	lw	$3,D_8013968C
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L13
	li	$17,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bgtz	$21,.L14
	move	$7,$21
	.set	macro
	.set	reorder

	move	$7,$17
.L14:
	addu	$4,$sp,40
	lbu	$6,8($20)
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	func_800A25D8
	sll	$6,$6,24
	.set	macro
	.set	reorder

	lw	$8,40($sp)
	lw	$9,44($sp)
	sw	$8,32($sp)
	sw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80048B04
	addu	$4,$sp,32
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beql	$2,$17,.L13
	move	$17,$0
	.set	macro
	.set	reorder

.L13:
	bne	$17,$0,.L10
.L12:
	lw	$3,D_8013968C
	li	$2,33			# 0x00000021
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L18
	move	$16,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80048EE0
	move	$4,$20
	.set	macro
	.set	reorder

	beq	$2,$0,.L18
	.set	noreorder
	.set	nomacro
	jal	func_800A6FD0
	move	$4,$20
	.set	macro
	.set	reorder

	sltu	$16,$2,1
.L18:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L19
	li	$2,34			# 0x00000022
	.set	macro
	.set	reorder

	sw	$2,D_8013968C
.L19:
	lw	$3,D_8013968C
	#nop
	sltu	$2,$3,73
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L229
	sltu	$2,$3,71
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L56
	sltu	$2,$3,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L10
	sltu	$2,$3,33
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L10
	andi	$2,$22,0x0080
	.set	macro
	.set	reorder

	j	.L23
.L229:
	li	$2,110			# 0x0000006e
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L56
	sltu	$2,$3,111
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L230
	li	$2,105			# 0x00000069
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L241
	move	$19,$0
	.set	macro
	.set	reorder

	j	.L10
.L230:
	li	$2,166			# 0x000000a6
	beq	$3,$2,.L208
	j	.L10
.L23:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L24
	andi	$2,$22,0x0100
	.set	macro
	.set	reorder

	lbu	$2,30($20)
	#nop
	andi	$2,$2,0x000c
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L26
	li	$16,181			# 0x000000b5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L26
	li	$16,7			# 0x00000007
	.set	macro
	.set	reorder

.L24:
	beq	$2,$0,.L27
	lbu	$2,30($20)
	#nop
	andi	$2,$2,0x000c
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L26
	li	$16,182			# 0x000000b6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L26
	li	$16,8			# 0x00000008
	.set	macro
	.set	reorder

.L27:
	lbu	$2,30($20)
	#nop
	andi	$2,$2,0x000c
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L26
	li	$16,180			# 0x000000b4
	.set	macro
	.set	reorder

	li	$16,6			# 0x00000006
.L26:
	lw	$3,D_8013968C
	li	$2,34			# 0x00000022
	beq	$3,$2,.L231
	jal	func_80084CB8
	la	$5,func_80087E4C
	.set	noreorder
	.set	nomacro
	jal	func_80084AD8
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L32
	li	$2,35			# 0x00000023
	.set	macro
	.set	reorder

	lw	$3,D_8013968C
	#nop
	beq	$3,$2,.L10
.L231:
	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	move	$4,$16
	.set	macro
	.set	reorder

	li	$3,2			# 0x00000002
	sw	$3,28($2)
	la	$4,func_8008865C
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	li	$5,6			# 0x00000006
	.set	macro
	.set	reorder

	j	.L10
.L32:
	lw	$3,D_8013968C
	#nop
	beq	$3,$2,.L34
	jal	func_80084A20
	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	move	$4,$16
	.set	macro
	.set	reorder

	li	$3,2			# 0x00000002
	.set	noreorder
	.set	nomacro
	jal	func_80084B80
	sw	$3,28($2)
	.set	macro
	.set	reorder

.L34:
	lbu	$2,30($20)
	#nop
	srl	$2,$2,2
	andi	$2,$2,0x0001
	beq	$2,$0,.L35
	lw	$4,D_801476B8
	#nop
	lw	$2,260($4)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L242
	andi	$2,$22,0x4000
	.set	macro
	.set	reorder

	jal	func_80048FD8
	andi	$2,$22,0x4000
.L242:
	beq	$2,$0,.L35
	jal	func_80084A20
	li	$4,364			# 0x0000016c
	move	$5,$20
	lbu	$6,8($20)
	.set	noreorder
	.set	nomacro
	jal	func_80050FA0
	li	$7,65536			# 0x00010000
	.set	macro
	.set	reorder

	jal	func_80084B80
	li	$4,346			# 0x0000015a
	move	$5,$20
	.set	noreorder
	.set	nomacro
	jal	func_80051264
	li	$6,65536			# 0x00010000
	.set	macro
	.set	reorder

	lw	$2,D_801476B8
	#nop
	lw	$2,260($2)
	#nop
	beq	$2,$0,.L10
	j	.L232
.L35:
	la	$4,func_800855E0
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	move	$5,$23
	.set	macro
	.set	reorder

	move	$5,$2
	andi	$2,$22,0x0c00
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L40
	sw	$21,44($5)
	.set	macro
	.set	reorder

	li	$2,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	j	.L41
	sw	$2,36($5)
	.set	macro
	.set	reorder

.L40:
	andi	$2,$22,0x1000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L42
	li	$2,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L41
	sw	$2,36($5)
	.set	macro
	.set	reorder

.L42:
	andi	$2,$22,0x2000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L44
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L41
	sw	$2,36($5)
	.set	macro
	.set	reorder

.L44:
	andi	$2,$22,0x8000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L46
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L41
	sw	$2,36($5)
	.set	macro
	.set	reorder

.L46:
	sw	$0,36($5)
.L41:
	lbu	$2,30($20)
	#nop
	andi	$2,$2,0x007c
	beq	$2,$0,.L10
	.set	noreorder
	.set	nomacro
	bne	$21,$0,.L10
	move	$4,$20
	.set	macro
	.set	reorder

	addu	$5,$sp,48
	.set	noreorder
	.set	nomacro
	jal	func_80049414
	addu	$6,$sp,52
	.set	macro
	.set	reorder

	la	$4,func_8008B9B0
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	move	$5,$23
	.set	macro
	.set	reorder

	lw	$3,48($sp)
	lw	$4,52($sp)
	move	$5,$2
	sw	$3,36($5)
	sw	$4,40($5)
	lbu	$2,30($20)
	#nop
	srl	$2,$2,2
	andi	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L49
	move	$3,$0
	.set	macro
	.set	reorder

	lw	$2,D_801476B8
	#nop
	lw	$2,260($2)
	#nop
	sltu	$3,$0,$2
.L49:
	beq	$3,$0,.L10
.L232:
	jal	func_80084A20
	jal	func_80083FF8
	la	$4,func_8008865C
	sll	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	andi	$5,$2,0xfffe
	.set	macro
	.set	reorder

	jal	func_800C5F60
	.set	noreorder
	.set	nomacro
	jal	func_80048FD8
	move	$4,$2
	.set	macro
	.set	reorder

	jal	func_80084B80
	j	.L10
.L56:
	move	$19,$0
.L241:
	sw	$0,60($sp)
	sw	$0,64($sp)
	lw	$2,0($20)
	lw	$3,D_8013968C
	move	$18,$19
	sw	$2,32($sp)
	lw	$2,4($20)
	#nop
	sw	$2,36($sp)
	li	$2,110			# 0x0000006e
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L57
	move	$fp,$21
	.set	macro
	.set	reorder

	li	$2,72			# 0x00000048
	beq	$3,$2,.L57
	.set	noreorder
	.set	nomacro
	jal	func_800A6FD0
	move	$4,$20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L57
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	addu	$17,$sp,40
.L58:
	slt	$2,$21,$16
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L59
	move	$4,$17
	.set	macro
	.set	reorder

	addu	$5,$sp,32
	lbu	$6,8($20)
	move	$7,$16
	.set	noreorder
	.set	nomacro
	jal	func_800A25D8
	sll	$6,$6,24
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800B4928
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L60
	addu	$18,$18,1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L58
	addu	$16,$16,1
	.set	macro
	.set	reorder

.L60:
.L59:
	lw	$3,D_8013968C
	li	$2,105			# 0x00000069
	bne	$3,$2,.L64
	li	$fp,1			# 0x00000001
	addu	$18,$18,$fp
.L64:
	lw	$2,D_80139688
	#nop
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L66
	move	$18,$0
	.set	macro
	.set	reorder

	jal	func_80048AE4
	xori	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L66
	move	$18,$0
	.set	macro
	.set	reorder

.L66:
	beq	$18,$0,.L57
	move	$17,$0
	lw	$2,D_801E4E70
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L71
	move	$16,$17
	.set	macro
	.set	reorder

	jal	func_800419C4
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L70
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	jal	func_80042990
	beq	$2,$0,.L70
.L71:
	li	$16,1			# 0x00000001
.L70:
	.set	noreorder
	.set	nomacro
	bnel	$16,$0,.L72
	li	$17,1			# 0x00000001
	.set	macro
	.set	reorder

.L72:
	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	func_800E49F4
	move	$5,$17
	.set	macro
	.set	reorder

	ori	$19,$19,0x2001
	.set	noreorder
	.set	nomacro
	jal	func_800840B0
	move	$4,$2
	.set	macro
	.set	reorder

.L57:
	lbu	$3,31($20)
	li	$2,213			# 0x000000d5
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L73
	li	$2,214			# 0x000000d6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L234
	li	$4,305			# 0x00000131
	.set	macro
	.set	reorder

.L73:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L74
	move	$18,$3
	.set	macro
	.set	reorder

	li	$4,311			# 0x00000137
.L234:
	move	$5,$20
	li	$6,65536			# 0x00010000
	.set	noreorder
	.set	nomacro
	jal	func_80051264
	or	$6,$19,$6
	.set	macro
	.set	reorder

	j	.L10
.L74:
	lbu	$8,8($20)
	li	$2,26			# 0x0000001a
	.set	noreorder
	.set	nomacro
	beq	$18,$2,.L80
	sw	$8,56($sp)
	.set	macro
	.set	reorder

	slt	$2,$18,27
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L207
	li	$2,23			# 0x00000017
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$18,$2,.L80
	addu	$3,$18,-24
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L248
	sltu	$2,$3,71
	.set	macro
	.set	reorder

.L207:
	slt	$2,$18,93
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L123
	slt	$2,$18,91
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L238
	addu	$3,$18,-24
	.set	macro
	.set	reorder

.L80:
	lw	$4,36($sp)
	lw	$5,32($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800625FC
	move	$23,$0
	.set	macro
	.set	reorder

	move	$3,$2
	andi	$2,$3,0x4000
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L243
	li	$9,1			# 0x00000001
	.set	macro
	.set	reorder

	li	$2,537919488			# 0x20100000
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L83
	move	$16,$23
	.set	macro
	.set	reorder

	jal	func_800627C4
	li	$3,3			# 0x00000003
	bne	$2,$3,.L83
	lbu	$2,D_80142F18+3
	#nop
	and	$2,$2,$3
	xori	$2,$2,0x0002
	sltu	$16,$2,1
.L83:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L81
	li	$9,1			# 0x00000001
	.set	macro
	.set	reorder

.L243:
	.set	noreorder
	.set	nomacro
	j	.L85
	sw	$9,68($sp)
	.set	macro
	.set	reorder

.L81:
	sw	$0,68($sp)
.L85:
	li	$3,92			# 0x0000005c
	.set	noreorder
	.set	nomacro
	beq	$18,$3,.L86
	li	$21,131072			# 0x00020000
	.set	macro
	.set	reorder

	li	$21,65536			# 0x00010000
.L86:
	lbu	$2,30($20)
	#nop
	srl	$2,$2,4
	andi	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L88
	li	$22,65536			# 0x00010000
	.set	macro
	.set	reorder

	li	$2,91			# 0x0000005b
	.set	noreorder
	.set	nomacro
	beq	$18,$2,.L89
	li	$17,89			# 0x00000059
	.set	macro
	.set	reorder

	bne	$18,$3,.L87
.L88:
	.set	noreorder
	.set	nomacro
	j	.L89
	li	$17,89			# 0x00000059
	.set	macro
	.set	reorder

.L87:
	.set	noreorder
	.set	nomacro
	jal	func_800E8978
	move	$4,$20
	.set	macro
	.set	reorder

	move	$3,$2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,.L90
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L91
	li	$17,89			# 0x00000059
	.set	macro
	.set	reorder

.L90:
	lbu	$4,0($3)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$4,$2,.L93
	li	$2,9			# 0x00000009
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$4,$2,.L95
	li	$2,105			# 0x00000069
	.set	macro
	.set	reorder

	j	.L96
.L93:
	lbu	$3,1($3)
	li	$2,109			# 0x0000006d
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L91
	li	$17,49			# 0x00000031
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L91
	li	$17,90			# 0x0000005a
	.set	macro
	.set	reorder

.L95:
	.set	noreorder
	.set	nomacro
	j	.L91
	li	$17,91			# 0x0000005b
	.set	macro
	.set	reorder

.L96:
	lbu	$17,1($3)
	lw	$3,D_8013968C
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L244
	li	$2,72			# 0x00000048
	.set	macro
	.set	reorder

	li	$2,73			# 0x00000049
	.set	noreorder
	.set	nomacro
	bne	$17,$2,.L97
	li	$2,88			# 0x00000058
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L91
	li	$17,92			# 0x0000005c
	.set	macro
	.set	reorder

.L97:
	.set	noreorder
	.set	nomacro
	bne	$17,$2,.L99
	li	$2,72			# 0x00000048
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L91
	li	$17,93			# 0x0000005d
	.set	macro
	.set	reorder

.L99:
	lw	$3,D_8013968C
.L244:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L245
	li	$2,76			# 0x0000004c
	.set	macro
	.set	reorder

	li	$2,68			# 0x00000044
	.set	noreorder
	.set	nomacro
	bne	$17,$2,.L101
	li	$2,76			# 0x0000004c
	.set	macro
	.set	reorder

	li	$17,94			# 0x0000005e
	ori	$21,$21,0x0800
	li	$10,1024			# 0x00000400
	sw	$10,64($sp)
	.set	noreorder
	.set	nomacro
	j	.L91
	ori	$22,$22,0x0800
	.set	macro
	.set	reorder

.L101:
.L245:
	.set	noreorder
	.set	nomacro
	bne	$17,$2,.L103
	li	$2,69			# 0x00000045
	.set	macro
	.set	reorder

	addu	$17,$fp,94
	ori	$21,$21,0x0800
	li	$11,1024			# 0x00000400
	sw	$11,64($sp)
	.set	noreorder
	.set	nomacro
	j	.L91
	ori	$22,$22,0x0800
	.set	macro
	.set	reorder

.L103:
	.set	noreorder
	.set	nomacro
	bne	$17,$2,.L105
	li	$8,2048			# 0x00000800
	.set	macro
	.set	reorder

	ori	$21,$21,0x0800
	li	$9,1024			# 0x00000400
	or	$22,$22,$8
	sw	$8,60($sp)
	.set	noreorder
	.set	nomacro
	j	.L91
	sw	$9,64($sp)
	.set	macro
	.set	reorder

.L105:
	lw	$10,68($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$10,$0,.L107
	li	$2,77			# 0x0000004d
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$17,$2,.L246
	li	$2,78			# 0x0000004e
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L91
	li	$17,98			# 0x00000062
	.set	macro
	.set	reorder

.L107:
	li	$2,78			# 0x0000004e
.L246:
	.set	noreorder
	.set	nomacro
	bne	$17,$2,.L247
	slt	$2,$17,50
	.set	macro
	.set	reorder

	addu	$16,$sp,40
	move	$4,$16
	lbu	$6,8($20)
	addu	$5,$sp,32
	.set	noreorder
	.set	nomacro
	jal	func_800A2594
	sll	$6,$6,24
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800B4928
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L247
	slt	$2,$17,50
	.set	macro
	.set	reorder

	lbu	$3,9($20)
	lbu	$2,9($2)
	andi	$3,$3,0x000f
	andi	$2,$2,0x000f
	slt	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L91
	li	$17,100			# 0x00000064
	.set	macro
	.set	reorder

.L91:
	slt	$2,$17,50
.L247:
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L113
	li	$17,99			# 0x00000063
	.set	macro
	.set	reorder

.L113:
	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	func_800E8A68
	li	$5,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L89
	move	$23,$0
	.set	macro
	.set	reorder

	lbu	$23,1($2)
.L89:
	li	$2,23			# 0x00000017
	.set	noreorder
	.set	nomacro
	beq	$18,$2,.L116
	li	$2,91			# 0x0000005b
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$18,$2,.L116
	li	$2,92			# 0x0000005c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$18,$2,.L115
	addu	$2,$17,-50
	.set	macro
	.set	reorder

.L116:
	addu	$2,$17,-50
	sll	$2,$2,2
	lw	$17,D_801393E4($2)
	.set	noreorder
	.set	nomacro
	j	.L239
	srl	$2,$17,14
	.set	macro
	.set	reorder

.L115:
	sll	$2,$2,2
	lw	$17,D_801394B0($2)
	#nop
	srl	$2,$17,14
.L239:
	andi	$2,$2,0x007c
	lw	$fp,D_80139334($2)
	.set	noreorder
	.set	nomacro
	beq	$23,$0,.L118
	srl	$2,$17,19
	.set	macro
	.set	reorder

	andi	$2,$2,0x001c
	lw	$18,D_801393C4($2)
	.set	noreorder
	.set	nomacro
	j	.L240
	srl	$2,$17,14
	.set	macro
	.set	reorder

.L118:
	move	$18,$0
	srl	$2,$17,14
.L240:
	andi	$2,$2,0x007c
	lw	$16,D_8013937C($2)
	.set	noreorder
	.set	nomacro
	jal	func_80084A68
	andi	$17,$17,0x7fff
	.set	macro
	.set	reorder

	la	$4,func_8008865C
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	move	$5,$0
	.set	macro
	.set	reorder

	jal	func_80084A20
	move	$4,$fp
	move	$5,$20
	.set	noreorder
	.set	nomacro
	jal	func_80051264
	or	$6,$19,$21
	.set	macro
	.set	reorder

	jal	func_80084B80
	jal	func_80084A20
	move	$5,$20
	move	$7,$22
	lw	$6,56($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80050FA0
	move	$4,$16
	.set	macro
	.set	reorder

	jal	func_80084B80
	lw	$11,68($sp)
	#nop
	.set	noreorder
	.set	nomacro
	bnel	$11,$0,.L120
	ori	$19,$19,0x0040
	.set	macro
	.set	reorder

.L120:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,.L121
	addu	$2,$23,-89
	.set	macro
	.set	reorder

	sll	$2,$2,2
	lhu	$16,D_8013F2EC($2)
	jal	func_80084A20
	move	$4,$18
	move	$5,$20
	li	$7,65536			# 0x00010000
	lw	$8,60($sp)
	lw	$6,56($sp)
	or	$2,$19,$8
	or	$7,$2,$7
	.set	noreorder
	.set	nomacro
	jal	func_800510E0
	sw	$16,16($sp)
	.set	macro
	.set	reorder

	jal	func_80084B80
.L121:
	.set	noreorder
	.set	nomacro
	beq	$17,$0,.L10
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$20
	lw	$6,56($sp)
	lw	$9,64($sp)
	li	$7,65536			# 0x00010000
	sw	$0,16($sp)
	or	$2,$19,$9
	.set	noreorder
	.set	nomacro
	jal	func_800510E0
	or	$7,$2,$7
	.set	macro
	.set	reorder

	j	.L10
.L123:
	addu	$3,$18,-24
.L238:
	sltu	$2,$3,71
.L248:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L124
	li	$2,27			# 0x0000001b
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$18,$2,.L125
	sltu	$2,$3,5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L235
	li	$2,262144			# 0x00040000
	.set	macro
	.set	reorder

.L125:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L126
	li	$2,65536			# 0x00010000
	.set	macro
	.set	reorder

.L235:
	or	$19,$19,$2
.L126:
	addu	$2,$18,-24
	sll	$2,$2,1
	lhu	$17,D_8013957C($2)
	#nop
	andi	$2,$17,0xf800
	andi	$3,$2,0xffff
	li	$2,16384			# 0x00004000
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L154
	andi	$17,$17,0x07ff
	.set	macro
	.set	reorder

	slt	$2,$3,16385
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L199
	li	$2,8192			# 0x00002000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L196
	slt	$2,$3,8193
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L200
	li	$2,4096			# 0x00001000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L144
	slt	$2,$3,4097
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L201
	li	$2,2048			# 0x00000800
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L130
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L201:
	li	$2,6144			# 0x00001800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L135
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L200:
	li	$2,12288			# 0x00003000
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L150
	slt	$2,$3,12289
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L202
	li	$2,10240			# 0x00002800
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L148
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L202:
	li	$2,14336			# 0x00003800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L152
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L199:
	li	$2,24576			# 0x00006000
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L190
	slt	$2,$3,24577
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L203
	li	$2,20480			# 0x00005000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L174
	slt	$2,$3,20481
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L204
	li	$2,18432			# 0x00004800
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L171
	addu	$16,$sp,40
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L237
	move	$4,$17
	.set	macro
	.set	reorder

.L204:
	li	$2,22528			# 0x00005800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L181
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L203:
	li	$2,28672			# 0x00007000
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L193
	slt	$2,$3,28673
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L205
	li	$2,26624			# 0x00006800
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L192
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L205:
	li	$2,30720			# 0x00007800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L195
	li	$2,32768			# 0x00008000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L194
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L130:
	.set	noreorder
	.set	nomacro
	bltz	$21,.L131
	slt	$2,$21,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L129
	li	$17,218			# 0x000000da
	.set	macro
	.set	reorder

	li	$2,2			# 0x00000002
	.set	noreorder
	.set	nomacro
	bne	$21,$2,.L129
	li	$17,220			# 0x000000dc
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L129
	li	$17,219			# 0x000000db
	.set	macro
	.set	reorder

.L131:
	.set	noreorder
	.set	nomacro
	j	.L129
	li	$17,220			# 0x000000dc
	.set	macro
	.set	reorder

.L135:
	lbu	$3,117($20)
	li	$2,2			# 0x00000002
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L138
	slt	$2,$3,3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L143
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L137
	li	$17,216			# 0x000000d8
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L236
	li	$2,131072			# 0x00020000
	.set	macro
	.set	reorder

.L143:
	li	$2,3			# 0x00000003
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L139
	li	$17,216			# 0x000000d8
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L236
	li	$2,131072			# 0x00020000
	.set	macro
	.set	reorder

.L137:
	.set	noreorder
	.set	nomacro
	j	.L129
	li	$17,212			# 0x000000d4
	.set	macro
	.set	reorder

.L138:
	.set	noreorder
	.set	nomacro
	j	.L129
	li	$17,214			# 0x000000d6
	.set	macro
	.set	reorder

.L139:
	.set	noreorder
	.set	nomacro
	j	.L236
	li	$2,65536			# 0x00010000
	.set	macro
	.set	reorder

.L144:
	lbu	$3,117($20)
	li	$2,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L129
	addu	$16,$sp,40
	.set	macro
	.set	reorder

	move	$4,$16
	lbu	$6,8($20)
	addu	$5,$sp,32
	.set	noreorder
	.set	nomacro
	jal	func_800A2594
	sll	$6,$6,24
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800B4928
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L237
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,9($20)
	lbu	$2,9($2)
	andi	$3,$3,0x000f
	andi	$2,$2,0x000f
	slt	$3,$3,$2
	.set	noreorder
	.set	nomacro
	bnel	$3,$0,.L129
	li	$17,186			# 0x000000ba
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L148:
	lbu	$2,117($20)
	#nop
	addu	$3,$2,-1
	slt	$2,$3,3
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L149
	li	$3,2			# 0x00000002
	.set	macro
	.set	reorder

.L149:
	.set	noreorder
	.set	nomacro
	j	.L237
	addu	$4,$17,$3
	.set	macro
	.set	reorder

.L150:
	lbu	$2,117($20)
	#nop
	addu	$3,$2,-1
	slt	$2,$3,4
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L151
	li	$3,3			# 0x00000003
	.set	macro
	.set	reorder

.L151:
	.set	noreorder
	.set	nomacro
	j	.L237
	addu	$4,$17,$3
	.set	macro
	.set	reorder

.L152:
	andi	$2,$22,0x0180
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L129
	li	$17,197			# 0x000000c5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L237
	move	$4,$17
	.set	macro
	.set	reorder

.L154:
	lbu	$3,117($20)
	#nop
	slt	$2,$3,20
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L156
	move	$7,$0
	.set	macro
	.set	reorder

	slt	$2,$3,30
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L156
	li	$7,1			# 0x00000001
	.set	macro
	.set	reorder

	slt	$2,$3,60
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L156
	li	$7,2			# 0x00000002
	.set	macro
	.set	reorder

	slt	$2,$3,70
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L156
	li	$7,3			# 0x00000003
	.set	macro
	.set	reorder

	slt	$2,$3,77
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L156
	li	$7,4			# 0x00000004
	.set	macro
	.set	reorder

	slt	$2,$3,88
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L156
	li	$7,5			# 0x00000005
	.set	macro
	.set	reorder

	slt	$2,$3,90
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L156
	li	$7,6			# 0x00000006
	.set	macro
	.set	reorder

	slt	$2,$3,99
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L156
	li	$7,8			# 0x00000008
	.set	macro
	.set	reorder

	li	$7,7			# 0x00000007
.L156:
	sw	$0,16($sp)
	move	$4,$17
	move	$5,$20
	.set	noreorder
	.set	nomacro
	jal	func_800512BC
	move	$6,$19
	.set	macro
	.set	reorder

	j	.L10
.L171:
	move	$4,$16
	lbu	$6,8($20)
	addu	$5,$sp,32
	.set	noreorder
	.set	nomacro
	jal	func_800A2594
	sll	$6,$6,24
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800B4928
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L237
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,9($20)
	lbu	$2,9($2)
	andi	$3,$3,0x000f
	andi	$2,$2,0x000f
	slt	$3,$3,$2
	.set	noreorder
	.set	nomacro
	bnel	$3,$0,.L129
	li	$17,181			# 0x000000b5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L174:
	lbu	$3,117($20)
	li	$2,2			# 0x00000002
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L176
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L177
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L249
	move	$5,$20
	.set	macro
	.set	reorder

.L176:
	.set	noreorder
	.set	nomacro
	j	.L129
	li	$17,285			# 0x0000011d
	.set	macro
	.set	reorder

.L177:
	.set	noreorder
	.set	nomacro
	j	.L129
	li	$17,286			# 0x0000011e
	.set	macro
	.set	reorder

.L181:
	lbu	$3,117($20)
	li	$2,2			# 0x00000002
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L184
	slt	$2,$3,3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L186
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L129
	li	$17,204			# 0x000000cc
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L129
	li	$17,202			# 0x000000ca
	.set	macro
	.set	reorder

.L184:
	.set	noreorder
	.set	nomacro
	j	.L129
	li	$17,203			# 0x000000cb
	.set	macro
	.set	reorder

.L186:
	.set	noreorder
	.set	nomacro
	j	.L129
	li	$17,204			# 0x000000cc
	.set	macro
	.set	reorder

.L190:
	.set	noreorder
	.set	nomacro
	j	.L191
	li	$16,271			# 0x0000010f
	.set	macro
	.set	reorder

.L192:
	.set	noreorder
	.set	nomacro
	j	.L191
	li	$16,272			# 0x00000110
	.set	macro
	.set	reorder

.L193:
	.set	noreorder
	.set	nomacro
	j	.L191
	li	$16,273			# 0x00000111
	.set	macro
	.set	reorder

.L194:
	.set	noreorder
	.set	nomacro
	j	.L191
	li	$16,277			# 0x00000115
	.set	macro
	.set	reorder

.L195:
	li	$16,274			# 0x00000112
.L191:
	jal	func_80084A68
	la	$4,func_8008B678
	.set	noreorder
	.set	nomacro
	jal	func_800850F8
	move	$5,$0
	.set	macro
	.set	reorder

	jal	func_80084A20
	move	$4,$16
	move	$5,$20
	lbu	$6,8($20)
	.set	noreorder
	.set	nomacro
	jal	func_80050FA0
	move	$7,$19
	.set	macro
	.set	reorder

	jal	func_80084B80
	.set	noreorder
	.set	nomacro
	j	.L237
	move	$4,$17
	.set	macro
	.set	reorder

.L196:
	li	$2,131072			# 0x00020000
.L236:
	or	$19,$19,$2
.L129:
	move	$4,$17
.L237:
	move	$5,$20
.L249:
	.set	noreorder
	.set	nomacro
	jal	func_80051264
	move	$6,$19
	.set	macro
	.set	reorder

	j	.L10
.L124:
	.set	noreorder
	.set	nomacro
	jal	func_800851B0
	li	$4,179			# 0x000000b3
	.set	macro
	.set	reorder

	la	$4,func_800874FC
	.set	noreorder
	.set	nomacro
	jal	func_80085154
	move	$5,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L10
	sh	$19,18($2)
	.set	macro
	.set	reorder

.L208:
	.set	noreorder
	.set	nomacro
	bne	$21,$0,.L209
	li	$4,-1			# 0xffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800A6FD0
	move	$4,$20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L10
	li	$4,50			# 0x00000032
	.set	macro
	.set	reorder

	j	.L211
.L209:
	.set	noreorder
	.set	nomacro
	bne	$22,$0,.L211
	addu	$5,$21,-2
	.set	macro
	.set	reorder

	sltu	$2,$5,18
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L211
	sll	$2,$5,2
	.set	macro
	.set	reorder

	lw	$2,.L225($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L225:
	.word	.L215
	.word	.L211
	.word	.L214
	.word	.L211
	.word	.L216
	.word	.L211
	.word	.L211
	.word	.L211
	.word	.L218
	.word	.L218
	.word	.L219
	.word	.L211
	.word	.L220
	.word	.L211
	.word	.L211
	.word	.L211
	.word	.L221
	.word	.L224
	.text
.L214:
	.set	noreorder
	.set	nomacro
	j	.L211
	li	$4,142			# 0x0000008e
	.set	macro
	.set	reorder

.L215:
	.set	noreorder
	.set	nomacro
	j	.L211
	li	$4,49			# 0x00000031
	.set	macro
	.set	reorder

.L216:
	.set	noreorder
	.set	nomacro
	j	.L211
	li	$4,55			# 0x00000037
	.set	macro
	.set	reorder

.L218:
	.set	noreorder
	.set	nomacro
	j	.L211
	li	$4,47			# 0x0000002f
	.set	macro
	.set	reorder

.L219:
	.set	noreorder
	.set	nomacro
	j	.L211
	li	$4,46			# 0x0000002e
	.set	macro
	.set	reorder

.L220:
	.set	noreorder
	.set	nomacro
	j	.L211
	li	$4,45			# 0x0000002d
	.set	macro
	.set	reorder

.L221:
	lbu	$2,30($20)
	#nop
	andi	$2,$2,0x007c
	beq	$2,$0,.L10
	.set	noreorder
	.set	nomacro
	jal	func_800E04D0
	move	$4,$20
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	li	$3,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	beq	$2,$3,.L10
	li	$4,51			# 0x00000033
	.set	macro
	.set	reorder

	j	.L211
.L224:
	li	$4,56			# 0x00000038
.L211:
	jal	func_800851B0
.L10:
	lw	$31,108($sp)
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,112
	.set	macro
	.set	reorder

	.end	func_8004D588
.Lfe1:
	.size	 func_8004D588,.Lfe1-func_8004D588
