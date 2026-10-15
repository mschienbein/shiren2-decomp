	.file	1 "source/src/units/main_14400/func_80062EA4.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_80062EA4
	.type	 func_80062EA4,@function
	.ent	func_80062EA4
func_80062EA4:
	.frame	$sp,72,$31		# vars= 16, regs= 10/0, args= 16, extra= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,72
	sw	$16,32($sp)
	lw	$16,96($sp)
	sw	$18,40($sp)
	move	$18,$4
	sw	$19,44($sp)
	move	$19,$5
	sw	$22,56($sp)
	move	$22,$6
	sw	$fp,64($sp)
	move	$fp,$7
	sw	$17,36($sp)
	sw	$31,68($sp)
	sw	$23,60($sp)
	sw	$21,52($sp)
	sw	$20,48($sp)
	sb	$0,28($sp)
	sb	$0,29($sp)
	lw	$2,44($19)
	lbu	$23,95($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L2
	move	$17,$0
	.set	macro
	.set	reorder

	addu	$4,$sp,16
	addu	$5,$sp,20
	.set	noreorder
	.set	nomacro
	jal	func_80059B58
	addu	$6,$sp,24
	.set	macro
	.set	reorder

	lw	$2,44($19)
	li	$13,1			# 0x00000001
	andi	$3,$2,0x0003
	li	$2,2			# 0x00000002
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L5
	sb	$13,28($sp)
	.set	macro
	.set	reorder

	sltu	$2,$3,3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L9
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L4
	move	$4,$18
	.set	macro
	.set	reorder

	j	.L2
.L9:
	li	$2,3			# 0x00000003
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L6
	move	$4,$18
	.set	macro
	.set	reorder

	j	.L2
.L4:
	li	$2,-633864192			# 0xda380000
	sw	$2,0($4)
	lw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	j	.L95
	addu	$18,$18,8
	.set	macro
	.set	reorder

.L5:
	move	$4,$18
	li	$2,-633864192			# 0xda380000
	sw	$2,0($4)
	lw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	j	.L95
	addu	$18,$18,8
	.set	macro
	.set	reorder

.L6:
	addu	$18,$18,8
	li	$2,-633864192			# 0xda380000
	sw	$2,0($4)
	lw	$2,24($sp)
.L95:
	li	$3,-2147483648			# 0x80000000
	addu	$2,$2,$3
	sw	$2,4($4)
.L2:
	lw	$2,48($19)
	#nop
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L108
	move	$2,$18
	.set	macro
	.set	reorder

	lw	$2,12($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L108
	move	$2,$18
	.set	macro
	.set	reorder

	lw	$2,72($22)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L10
	li	$13,1			# 0x00000001
	.set	macro
	.set	reorder

	move	$4,$18
	addu	$18,$18,8
	li	$2,-633864192			# 0xda380000
	sb	$13,29($sp)
	sw	$2,0($4)
	lw	$2,72($22)
	li	$3,-2147483648			# 0x80000000
	addu	$2,$2,$3
	sw	$2,4($4)
.L10:
	move	$2,$18
.L108:
	li	$3,-419430400			# 0xe7000000
	sw	$3,0($2)
	sw	$0,4($2)
	lw	$2,28($19)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L11
	addu	$18,$18,8
	.set	macro
	.set	reorder

	move	$3,$18
	addu	$18,$18,8
	li	$2,-486539264			# 0xe3000000
	ori	$2,$2,0x0a01
	sw	$2,0($3)
	li	$2,1048576			# 0x00100000
	.set	noreorder
	.set	nomacro
	j	.L12
	sw	$2,4($3)
	.set	macro
	.set	reorder

.L11:
	move	$3,$18
	addu	$18,$18,8
	li	$2,-486539264			# 0xe3000000
	ori	$2,$2,0x0a01
	sw	$2,0($3)
	sw	$0,4($3)
.L12:
	move	$5,$18
	addu	$18,$18,8
	li	$2,-503316480			# 0xe2000000
	ori	$2,$2,0x001c
	move	$4,$18
	addu	$18,$18,8
	li	$7,-637599744			# 0xd9ff0000
	ori	$7,$7,0xffff
	sw	$2,0($5)
	lw	$2,36($19)
	lw	$3,40($19)
	li	$6,-131072			# 0xfffe0000
	or	$2,$2,$3
	sw	$2,4($5)
	li	$2,-654245888			# 0xd9010000
	move	$3,$18
	sw	$2,0($4)
	sw	$0,4($4)
	sw	$7,0($3)
	lw	$2,32($19)
	ori	$6,$6,0xffff
	and	$2,$2,$6
	sw	$2,4($3)
	lw	$4,48($19)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$4,$0,.L13
	addu	$18,$18,8
	.set	macro
	.set	reorder

	lw	$2,8($4)
	#nop
	bne	$2,$0,.L14
	lw	$2,4($4)
	#nop
	beq	$2,$0,.L13
.L14:
	lw	$5,4($4)
	#nop
	beq	$5,$0,.L15
	bltz	$16,.L15
	lw	$6,36($4)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$6,$0,.L16
	sll	$2,$16,1
	.set	macro
	.set	reorder

	addu	$2,$2,$16
	sll	$2,$2,2
	addu	$2,$2,$5
	lw	$3,8($2)
	lw	$2,40($4)
	#nop
	mult	$3,$2
	mflo	$13
	#nop
	#nop
	addu	$17,$6,$13
.L16:
	sll	$2,$16,1
	addu	$2,$2,$16
	sll	$2,$2,2
	addu	$2,$2,$5
	lw	$3,4($2)
	j	.L96
.L15:
	lw	$4,48($19)
	#nop
	lw	$5,36($4)
	#nop
	beq	$5,$0,.L18
	lh	$3,0($22)
	lw	$2,40($4)
	#nop
	mult	$3,$2
	mflo	$13
	#nop
	#nop
	addu	$17,$5,$13
.L18:
	lh	$3,2($22)
.L96:
	lw	$2,32($4)
	#nop
	mult	$3,$2
	lw	$2,28($4)
	mflo	$13
	#nop
	#nop
	addu	$12,$2,$13
	lw	$2,48($19)
	#nop
	lw	$2,16($2)
	#nop
	andi	$3,$2,0x0f00
	li	$2,256			# 0x00000100
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L22
	sltu	$2,$3,257
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L19
	move	$11,$0
	.set	macro
	.set	reorder

	li	$2,512			# 0x00000200
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L23
	li	$2,1024			# 0x00000400
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beql	$3,$2,.L24
	li	$11,3			# 0x00000003
	.set	macro
	.set	reorder

	j	.L19
.L22:
	.set	noreorder
	.set	nomacro
	j	.L19
	li	$11,1			# 0x00000001
	.set	macro
	.set	reorder

.L23:
	li	$11,2			# 0x00000002
.L24:
.L19:
	lw	$2,48($19)
	#nop
	lw	$2,16($2)
	#nop
	andi	$3,$2,0x000f
	sltu	$2,$3,6
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L33
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lw	$2,.L34($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L34:
	.word	.L28
	.word	.L29
	.word	.L30
	.word	.L33
	.word	.L32
	.word	.L32
	.text
.L28:
	.set	noreorder
	.set	nomacro
	j	.L27
	li	$20,4			# 0x00000004
	.set	macro
	.set	reorder

.L29:
	.set	noreorder
	.set	nomacro
	j	.L27
	li	$20,3			# 0x00000003
	.set	macro
	.set	reorder

.L30:
	.set	noreorder
	.set	nomacro
	j	.L27
	li	$20,2			# 0x00000002
	.set	macro
	.set	reorder

.L32:
	.set	noreorder
	.set	nomacro
	j	.L27
	move	$20,$0
	.set	macro
	.set	reorder

.L33:
	li	$20,1			# 0x00000001
.L27:
	lw	$2,48($19)
	#nop
	lw	$2,36($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L35
	li	$2,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$20,$2,.L36
	move	$3,$18
	.set	macro
	.set	reorder

	addu	$18,$18,8
	li	$2,-486539264			# 0xe3000000
	ori	$2,$2,0x1001
	sw	$2,0($3)
	li	$2,32768			# 0x00008000
	sw	$2,4($3)
.L36:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,.L38
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$11,$2,.L39
	move	$3,$18
	.set	macro
	.set	reorder

	j	.L106
.L38:
	move	$3,$18
	addu	$18,$18,8
	move	$4,$18
	addu	$18,$18,8
	move	$7,$18
	addu	$18,$18,8
	li	$9,-184549376			# 0xf5000000
	ori	$9,$9,0x0100
	move	$5,$18
	addu	$18,$18,8
	move	$8,$18
	addu	$18,$18,8
	.set	noreorder
	.set	nomacro
	j	.L97
	li	$10,117637120			# 0x07030000
	.set	macro
	.set	reorder

.L39:
	addu	$18,$18,8
	move	$4,$18
	addu	$18,$18,8
	move	$7,$18
	addu	$18,$18,8
	li	$9,-184549376			# 0xf5000000
	ori	$9,$9,0x0100
	move	$5,$18
	addu	$18,$18,8
	move	$8,$18
	addu	$18,$18,8
	li	$10,121569280			# 0x073f0000
.L97:
	ori	$10,$10,0xc000
	move	$6,$18
	addu	$18,$18,8
	li	$2,-49283072			# 0xfd100000
	sw	$2,0($3)
	li	$2,-402653184			# 0xe8000000
	sw	$17,4($3)
	sw	$2,0($4)
	li	$2,117440512			# 0x07000000
	sw	$0,4($4)
	sw	$2,4($7)
	li	$2,-436207616			# 0xe6000000
	sw	$9,0($7)
	sw	$2,0($5)
	li	$2,-268435456			# 0xf0000000
	sw	$0,4($5)
	sw	$2,0($8)
	li	$2,-419430400			# 0xe7000000
	sw	$10,4($8)
	sw	$2,0($6)
	.set	noreorder
	.set	nomacro
	j	.L42
	sw	$0,4($6)
	.set	macro
	.set	reorder

.L35:
	move	$3,$18
	addu	$18,$18,8
	li	$2,-486539264			# 0xe3000000
	ori	$2,$2,0x1001
	sw	$2,0($3)
	sw	$0,4($3)
.L42:
	li	$2,1			# 0x00000001
.L106:
	.set	noreorder
	.set	nomacro
	beq	$11,$2,.L51
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$11,$0,.L110
	li	$2,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$11,$2,.L111
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beql	$11,$2,.L65
	addu	$18,$18,8
	.set	macro
	.set	reorder

	j	.L43
.L110:
	addu	$18,$18,8
	sll	$3,$20,21
	li	$2,-49283072			# 0xfd100000
	or	$2,$3,$2
	move	$17,$18
	sw	$2,0($4)
	li	$2,-183500800			# 0xf5100000
	or	$3,$3,$2
	sw	$12,4($4)
	sw	$3,0($17)
	lw	$2,48($19)
	#nop
	lw	$4,24($2)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	addu	$18,$18,8
	.set	macro
	.set	reorder

	lw	$3,48($19)
	#nop
	lw	$4,20($3)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	move	$16,$2
	.set	macro
	.set	reorder

	move	$3,$18
	addu	$18,$18,8
	andi	$16,$16,0x000f
	sll	$16,$16,14
	andi	$2,$2,0x000f
	sll	$2,$2,4
	li	$7,117440512			# 0x07000000
	or	$2,$2,$7
	or	$16,$16,$2
	li	$2,-436207616			# 0xe6000000
	move	$6,$18
	sw	$16,4($17)
	sw	$2,0($3)
	li	$2,-218103808			# 0xf3000000
	sw	$0,4($3)
	sw	$2,0($6)
	lw	$2,48($19)
	addu	$18,$18,8
	lw	$4,20($2)
	#nop
	srl	$5,$4,4
	.set	noreorder
	.set	nomacro
	beq	$5,$0,.L45
	li	$3,2048			# 0x00000800
	.set	macro
	.set	reorder

	addu	$3,$5,2047
.L45:
	lw	$2,24($2)
	#nop
	mult	$4,$2
	mflo	$13
	#nop
	#nop
	addu	$2,$13,3
	srl	$2,$2,2
	addu	$4,$2,-1
	sltu	$2,$4,2048
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L47
	li	$4,2047			# 0x000007ff
	.set	macro
	.set	reorder

.L47:
	andi	$2,$4,0x0fff
	sll	$2,$2,12
	.set	noreorder
	.set	nomacro
	beq	$5,$0,.L48
	or	$4,$2,$7
	.set	macro
	.set	reorder

	divu	$2,$3,$5
	.set	noreorder
	.set	nomacro
	j	.L98
	andi	$2,$2,0x0fff
	.set	macro
	.set	reorder

.L48:
	andi	$2,$3,0x0fff
.L98:
	or	$2,$4,$2
	move	$3,$18
	addu	$18,$18,8
	sw	$2,4($6)
	li	$2,-419430400			# 0xe7000000
	move	$17,$18
	sw	$2,0($3)
	sw	$0,4($3)
	lw	$2,48($19)
	sll	$20,$20,21
	lw	$2,20($2)
	li	$21,-184549376			# 0xf5000000
	srl	$2,$2,1
	addu	$2,$2,7
	srl	$2,$2,3
	andi	$2,$2,0x01ff
	sll	$2,$2,9
	or	$2,$2,$21
	or	$2,$20,$2
	sw	$2,0($17)
	lw	$2,48($19)
	#nop
	lw	$4,24($2)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	addu	$18,$18,8
	.set	macro
	.set	reorder

	lw	$3,48($19)
	#nop
	lw	$4,20($3)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	move	$16,$2
	.set	macro
	.set	reorder

	andi	$16,$16,0x000f
	sll	$16,$16,14
	andi	$2,$2,0x000f
	sll	$2,$2,4
	or	$16,$16,$2
	move	$4,$18
	li	$2,-234881024			# 0xf2000000
	sw	$16,4($17)
	sw	$2,0($4)
	lw	$2,48($19)
	addu	$18,$18,8
	lw	$3,20($2)
	lw	$2,24($2)
	addu	$3,$3,-1
	sll	$3,$3,2
	andi	$3,$3,0x0fff
	sll	$3,$3,12
	addu	$2,$2,-1
	sll	$2,$2,2
	andi	$2,$2,0x0fff
	or	$3,$3,$2
	andi	$2,$23,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L43
	sw	$3,4($4)
	.set	macro
	.set	reorder

	lw	$2,48($19)
	#nop
	lw	$2,20($2)
	move	$17,$18
	.set	noreorder
	.set	nomacro
	j	.L99
	srl	$2,$2,1
	.set	macro
	.set	reorder

.L51:
	addu	$18,$18,8
	sll	$3,$20,21
	li	$2,-49283072			# 0xfd100000
	or	$2,$3,$2
	move	$17,$18
	sw	$2,0($4)
	li	$2,-183500800			# 0xf5100000
	or	$3,$3,$2
	sw	$12,4($4)
	sw	$3,0($17)
	lw	$2,48($19)
	#nop
	lw	$4,24($2)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	addu	$18,$18,8
	.set	macro
	.set	reorder

	lw	$3,48($19)
	#nop
	lw	$4,20($3)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	move	$16,$2
	.set	macro
	.set	reorder

	move	$3,$18
	addu	$18,$18,8
	andi	$16,$16,0x000f
	sll	$16,$16,14
	andi	$2,$2,0x000f
	sll	$2,$2,4
	li	$7,117440512			# 0x07000000
	or	$2,$2,$7
	or	$16,$16,$2
	li	$2,-436207616			# 0xe6000000
	move	$6,$18
	sw	$16,4($17)
	sw	$2,0($3)
	li	$2,-218103808			# 0xf3000000
	sw	$0,4($3)
	sw	$2,0($6)
	lw	$2,48($19)
	addu	$18,$18,8
	lw	$4,20($2)
	#nop
	srl	$5,$4,3
	.set	noreorder
	.set	nomacro
	beq	$5,$0,.L52
	li	$3,2048			# 0x00000800
	.set	macro
	.set	reorder

	addu	$3,$5,2047
.L52:
	lw	$2,24($2)
	#nop
	mult	$4,$2
	mflo	$13
	#nop
	#nop
	addu	$2,$13,1
	srl	$2,$2,1
	addu	$4,$2,-1
	sltu	$2,$4,2048
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L54
	li	$4,2047			# 0x000007ff
	.set	macro
	.set	reorder

.L54:
	andi	$2,$4,0x0fff
	sll	$2,$2,12
	.set	noreorder
	.set	nomacro
	beq	$5,$0,.L55
	or	$4,$2,$7
	.set	macro
	.set	reorder

	divu	$2,$3,$5
	.set	noreorder
	.set	nomacro
	j	.L100
	andi	$2,$2,0x0fff
	.set	macro
	.set	reorder

.L55:
	andi	$2,$3,0x0fff
.L100:
	or	$2,$4,$2
	move	$3,$18
	addu	$18,$18,8
	sw	$2,4($6)
	li	$2,-419430400			# 0xe7000000
	move	$17,$18
	sw	$2,0($3)
	sw	$0,4($3)
	lw	$2,48($19)
	sll	$20,$20,21
	lw	$2,20($2)
	li	$21,-184025088			# 0xf5080000
	addu	$2,$2,7
	srl	$2,$2,3
	andi	$2,$2,0x01ff
	sll	$2,$2,9
	or	$2,$2,$21
	or	$2,$20,$2
	sw	$2,0($17)
	lw	$2,48($19)
	#nop
	lw	$4,24($2)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	addu	$18,$18,8
	.set	macro
	.set	reorder

	lw	$3,48($19)
	#nop
	lw	$4,20($3)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	move	$16,$2
	.set	macro
	.set	reorder

	andi	$16,$16,0x000f
	sll	$16,$16,14
	andi	$2,$2,0x000f
	sll	$2,$2,4
	or	$16,$16,$2
	move	$4,$18
	li	$2,-234881024			# 0xf2000000
	sw	$16,4($17)
	sw	$2,0($4)
	lw	$2,48($19)
	addu	$18,$18,8
	lw	$3,20($2)
	lw	$2,24($2)
	addu	$3,$3,-1
	sll	$3,$3,2
	andi	$3,$3,0x0fff
	sll	$3,$3,12
	addu	$2,$2,-1
	sll	$2,$2,2
	andi	$2,$2,0x0fff
	or	$3,$3,$2
	andi	$2,$23,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L43
	sw	$3,4($4)
	.set	macro
	.set	reorder

	lw	$2,48($19)
	#nop
	lw	$2,20($2)
	.set	noreorder
	.set	nomacro
	j	.L99
	move	$17,$18
	.set	macro
	.set	reorder

.L111:
	addu	$18,$18,8
	sll	$3,$20,21
	li	$2,-49283072			# 0xfd100000
	or	$2,$3,$2
	move	$17,$18
	sw	$2,0($4)
	li	$2,-183500800			# 0xf5100000
	or	$3,$3,$2
	sw	$12,4($4)
	sw	$3,0($17)
	lw	$2,48($19)
	#nop
	lw	$4,24($2)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	addu	$18,$18,8
	.set	macro
	.set	reorder

	lw	$3,48($19)
	#nop
	lw	$4,20($3)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	move	$16,$2
	.set	macro
	.set	reorder

	move	$3,$18
	addu	$18,$18,8
	andi	$16,$16,0x000f
	sll	$16,$16,14
	andi	$2,$2,0x000f
	sll	$2,$2,4
	li	$7,117440512			# 0x07000000
	or	$2,$2,$7
	or	$16,$16,$2
	li	$2,-436207616			# 0xe6000000
	move	$6,$18
	sw	$16,4($17)
	sw	$2,0($3)
	li	$2,-218103808			# 0xf3000000
	sw	$0,4($3)
	sw	$2,0($6)
	lw	$2,48($19)
	addu	$18,$18,8
	lw	$4,20($2)
	#nop
	srl	$5,$4,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,.L59
	li	$3,2048			# 0x00000800
	.set	macro
	.set	reorder

	addu	$3,$5,2047
.L59:
	lw	$2,24($2)
	#nop
	mult	$4,$2
	mflo	$13
	#nop
	#nop
	addu	$4,$13,-1
	sltu	$2,$4,2048
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L61
	li	$4,2047			# 0x000007ff
	.set	macro
	.set	reorder

.L61:
	andi	$2,$4,0x0fff
	sll	$2,$2,12
	.set	noreorder
	.set	nomacro
	beq	$5,$0,.L62
	or	$4,$2,$7
	.set	macro
	.set	reorder

	divu	$2,$3,$5
	.set	noreorder
	.set	nomacro
	j	.L101
	andi	$2,$2,0x0fff
	.set	macro
	.set	reorder

.L62:
	andi	$2,$3,0x0fff
.L101:
	or	$2,$4,$2
	move	$3,$18
	addu	$18,$18,8
	sw	$2,4($6)
	li	$2,-419430400			# 0xe7000000
	move	$17,$18
	sw	$2,0($3)
	sw	$0,4($3)
	lw	$2,48($19)
	sll	$20,$20,21
	lw	$2,20($2)
	.set	noreorder
	.set	nomacro
	j	.L102
	li	$21,-183500800			# 0xf5100000
	.set	macro
	.set	reorder

.L65:
	sll	$3,$20,21
	li	$2,-48758784			# 0xfd180000
	or	$2,$3,$2
	move	$17,$18
	sw	$2,0($4)
	li	$2,-182976512			# 0xf5180000
	or	$3,$3,$2
	sw	$12,4($4)
	sw	$3,0($17)
	lw	$2,48($19)
	#nop
	lw	$4,24($2)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	addu	$18,$18,8
	.set	macro
	.set	reorder

	lw	$3,48($19)
	#nop
	lw	$4,20($3)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	move	$16,$2
	.set	macro
	.set	reorder

	move	$3,$18
	addu	$18,$18,8
	andi	$16,$16,0x000f
	sll	$16,$16,14
	andi	$2,$2,0x000f
	sll	$2,$2,4
	li	$7,117440512			# 0x07000000
	or	$2,$2,$7
	or	$16,$16,$2
	li	$2,-436207616			# 0xe6000000
	move	$6,$18
	sw	$16,4($17)
	sw	$2,0($3)
	li	$2,-218103808			# 0xf3000000
	sw	$0,4($3)
	sw	$2,0($6)
	lw	$2,48($19)
	addu	$18,$18,8
	lw	$4,20($2)
	#nop
	srl	$5,$4,1
	.set	noreorder
	.set	nomacro
	beq	$5,$0,.L66
	li	$3,2048			# 0x00000800
	.set	macro
	.set	reorder

	addu	$3,$5,2047
.L66:
	lw	$2,24($2)
	#nop
	mult	$4,$2
	mflo	$13
	#nop
	#nop
	addu	$4,$13,-1
	sltu	$2,$4,2048
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L68
	li	$4,2047			# 0x000007ff
	.set	macro
	.set	reorder

.L68:
	andi	$2,$4,0x0fff
	sll	$2,$2,12
	.set	noreorder
	.set	nomacro
	beq	$5,$0,.L69
	or	$4,$2,$7
	.set	macro
	.set	reorder

	divu	$2,$3,$5
	.set	noreorder
	.set	nomacro
	j	.L103
	andi	$2,$2,0x0fff
	.set	macro
	.set	reorder

.L69:
	andi	$2,$3,0x0fff
.L103:
	or	$2,$4,$2
	move	$3,$18
	addu	$18,$18,8
	sw	$2,4($6)
	li	$2,-419430400			# 0xe7000000
	move	$17,$18
	sw	$2,0($3)
	sw	$0,4($3)
	lw	$2,48($19)
	sll	$20,$20,21
	lw	$2,20($2)
	li	$21,-182976512			# 0xf5180000
.L102:
	sll	$2,$2,1
	addu	$2,$2,7
	srl	$2,$2,3
	andi	$2,$2,0x01ff
	sll	$2,$2,9
	or	$2,$2,$21
	or	$2,$20,$2
	sw	$2,0($17)
	lw	$2,48($19)
	#nop
	lw	$4,24($2)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	addu	$18,$18,8
	.set	macro
	.set	reorder

	lw	$3,48($19)
	#nop
	lw	$4,20($3)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	move	$16,$2
	.set	macro
	.set	reorder

	andi	$16,$16,0x000f
	sll	$16,$16,14
	andi	$2,$2,0x000f
	sll	$2,$2,4
	or	$16,$16,$2
	move	$4,$18
	li	$2,-234881024			# 0xf2000000
	sw	$16,4($17)
	sw	$2,0($4)
	lw	$2,48($19)
	addu	$18,$18,8
	lw	$3,20($2)
	lw	$2,24($2)
	addu	$3,$3,-1
	sll	$3,$3,2
	andi	$3,$3,0x0fff
	sll	$3,$3,12
	addu	$2,$2,-1
	sll	$2,$2,2
	andi	$2,$2,0x0fff
	or	$3,$3,$2
	andi	$2,$23,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L43
	sw	$3,4($4)
	.set	macro
	.set	reorder

	lw	$2,48($19)
	#nop
	lw	$2,20($2)
	move	$17,$18
	sll	$2,$2,1
.L99:
	addu	$2,$2,7
	srl	$2,$2,3
	andi	$2,$2,0x01ff
	sll	$2,$2,9
	or	$2,$2,$21
	or	$2,$20,$2
	sw	$2,0($17)
	lw	$2,48($19)
	#nop
	lw	$4,24($2)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	addu	$18,$18,8
	.set	macro
	.set	reorder

	lw	$3,48($19)
	#nop
	lw	$4,20($3)
	.set	noreorder
	.set	nomacro
	jal	func_80062CF4
	move	$16,$2
	.set	macro
	.set	reorder

	andi	$16,$16,0x000f
	sll	$16,$16,14
	andi	$2,$2,0x000f
	sll	$2,$2,4
	li	$3,16777216			# 0x01000000
	or	$2,$2,$3
	or	$16,$16,$2
	sw	$16,4($17)
.L43:
	lw	$4,48($19)
	move	$6,$18
	lw	$2,20($4)
	#nop
	sll	$3,$2,2
	mtc1	$3,$f2
	cvt.d.w	$f2,$f2
	.set	noreorder
	.set	nomacro
	bgez	$3,.L74
	addu	$18,$18,8
	.set	macro
	.set	reorder

	li.d	$f0,4.29496729600000000000e9
	#nop
	add.d	$f2,$f2,$f0
.L74:
	l.s	$f0,4($22)
	cvt.s.d	$f2,$f2
	mul.s	$f0,$f0,$f2
	nop
	trunc.w.s $f6,$f0,$13
	mfc1	$2,$f6
	#nop
	remu	$3,$2,$3
	lw	$2,24($4)
	#nop
	sll	$4,$2,2
	mtc1	$4,$f2
	cvt.d.w	$f2,$f2
	andi	$3,$3,0x0fff
	.set	noreorder
	.set	nomacro
	bgez	$4,.L75
	sll	$5,$3,12
	.set	macro
	.set	reorder

	li.d	$f0,4.29496729600000000000e9
	#nop
	add.d	$f2,$f2,$f0
.L75:
	l.s	$f0,8($22)
	cvt.s.d	$f2,$f2
	mul.s	$f0,$f0,$f2
	nop
	trunc.w.s $f6,$f0,$13
	mfc1	$2,$f6
	#nop
	remu	$3,$2,$4
	li	$7,-234881024			# 0xf2000000
	andi	$3,$3,0x0fff
	or	$3,$3,$7
	or	$3,$5,$3
	sw	$3,0($6)
	lw	$2,48($19)
	#nop
	lw	$3,20($2)
	lw	$2,24($2)
	addu	$3,$3,-1
	sll	$3,$3,2
	andi	$3,$3,0x0fff
	sll	$3,$3,12
	addu	$2,$2,-1
	sll	$2,$2,2
	andi	$2,$2,0x0fff
	or	$3,$3,$2
	sw	$3,4($6)
	andi	$3,$23,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$3,$0,.L13
	li	$2,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L78
	slt	$2,$3,3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L87
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L81
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L83
	move	$5,$18
	.set	macro
	.set	reorder

	j	.L87
.L78:
	lw	$4,48($19)
	move	$5,$18
	lw	$2,20($4)
	#nop
	sll	$3,$2,2
	mtc1	$3,$f2
	cvt.d.w	$f2,$f2
	.set	noreorder
	.set	nomacro
	bgez	$3,.L79
	addu	$18,$18,8
	.set	macro
	.set	reorder

	li.d	$f0,4.29496729600000000000e9
	#nop
	add.d	$f2,$f2,$f0
.L79:
	l.s	$f0,8($22)
	cvt.s.d	$f2,$f2
	mul.s	$f0,$f0,$f2
	nop
	trunc.w.s $f6,$f0,$13
	mfc1	$2,$f6
	#nop
	remu	$3,$2,$3
	li.d	$f2,5.00000000000000000000e-1
	lw	$2,24($4)
	l.s	$f0,4($22)
	.set	noreorder
	.set	nomacro
	j	.L107
	sll	$2,$2,2
	.set	macro
	.set	reorder

.L81:
	move	$5,$18
	addu	$18,$18,8
	lw	$2,48($19)
	l.s	$f0,4($22)
	li.d	$f2,5.00000000000000000000e-1
	cvt.d.s	$f0,$f0
	lw	$2,20($2)
	#nop
	sll	$2,$2,2
	mtc1	$2,$f4
	cvt.d.w	$f4,$f4
	.set	noreorder
	.set	nomacro
	bgez	$2,.L82
	add.d	$f2,$f0,$f2
	.set	macro
	.set	reorder

	li.d	$f0,4.29496729600000000000e9
	#nop
	add.d	$f4,$f4,$f0
.L82:
	mul.d	$f0,$f2,$f4
	nop
	trunc.w.d $f6,$f0,$13
	mfc1	$3,$f6
	#nop
	remu	$2,$3,$2
	andi	$2,$2,0x0fff
	sll	$2,$2,12
	.set	noreorder
	.set	nomacro
	j	.L104
	or	$2,$2,$7
	.set	macro
	.set	reorder

.L83:
	addu	$18,$18,8
	lw	$4,48($19)
	l.s	$f0,4($22)
	lw	$2,20($4)
	#nop
	sll	$3,$2,2
	mtc1	$3,$f2
	cvt.d.w	$f2,$f2
	.set	noreorder
	.set	nomacro
	bgez	$3,.L84
	neg.s	$f4,$f0
	.set	macro
	.set	reorder

	li.d	$f0,4.29496729600000000000e9
	#nop
	add.d	$f2,$f2,$f0
.L84:
	cvt.s.d	$f0,$f2
	mul.s	$f0,$f4,$f0
	nop
	trunc.w.s $f6,$f0,$13
	mfc1	$2,$f6
	#nop
	remu	$3,$2,$3
	li.d	$f2,5.00000000000000000000e-1
	lw	$2,24($4)
	l.s	$f0,8($22)
	sll	$2,$2,2
.L107:
	mtc1	$2,$f4
	cvt.d.w	$f4,$f4
	neg.s	$f0,$f0
	cvt.d.s	$f0,$f0
	add.d	$f2,$f0,$f2
	andi	$3,$3,0x0fff
	.set	noreorder
	.set	nomacro
	bgez	$2,.L85
	sll	$4,$3,12
	.set	macro
	.set	reorder

	li.d	$f0,4.29496729600000000000e9
	#nop
	add.d	$f4,$f4,$f0
.L85:
	mul.d	$f0,$f2,$f4
	nop
	trunc.w.d $f6,$f0,$13
	mfc1	$3,$f6
	#nop
	remu	$2,$3,$2
	andi	$2,$2,0x0fff
	or	$2,$2,$7
	or	$2,$4,$2
.L104:
	sw	$2,0($5)
	lw	$2,48($19)
	li	$4,16777216			# 0x01000000
	lw	$3,20($2)
	lw	$2,24($2)
	addu	$3,$3,-1
	sll	$3,$3,2
	andi	$3,$3,0x0fff
	sll	$3,$3,12
	addu	$2,$2,-1
	sll	$2,$2,2
	andi	$2,$2,0x0fff
	or	$2,$2,$4
	or	$3,$3,$2
	.set	noreorder
	.set	nomacro
	j	.L13
	sw	$3,4($5)
	.set	macro
	.set	reorder

.L87:
	lw	$4,48($19)
	move	$6,$18
	lw	$2,20($4)
	#nop
	sll	$3,$2,2
	mtc1	$3,$f2
	cvt.d.w	$f2,$f2
	.set	noreorder
	.set	nomacro
	bgez	$3,.L88
	addu	$18,$18,8
	.set	macro
	.set	reorder

	li.d	$f0,4.29496729600000000000e9
	#nop
	add.d	$f2,$f2,$f0
.L88:
	l.s	$f0,4($22)
	cvt.s.d	$f2,$f2
	mul.s	$f0,$f0,$f2
	nop
	trunc.w.s $f6,$f0,$13
	mfc1	$2,$f6
	#nop
	remu	$3,$2,$3
	lw	$2,24($4)
	#nop
	sll	$4,$2,2
	mtc1	$4,$f2
	cvt.d.w	$f2,$f2
	andi	$3,$3,0x0fff
	.set	noreorder
	.set	nomacro
	bgez	$4,.L89
	sll	$5,$3,12
	.set	macro
	.set	reorder

	li.d	$f0,4.29496729600000000000e9
	#nop
	add.d	$f2,$f2,$f0
.L89:
	l.s	$f0,8($22)
	cvt.s.d	$f2,$f2
	mul.s	$f0,$f0,$f2
	nop
	trunc.w.s $f6,$f0,$13
	mfc1	$2,$f6
	#nop
	remu	$3,$2,$4
	li	$2,-234881024			# 0xf2000000
	andi	$3,$3,0x0fff
	or	$3,$3,$2
	or	$3,$5,$3
	sw	$3,0($6)
	lw	$2,48($19)
	li	$4,16777216			# 0x01000000
	lw	$3,20($2)
	lw	$2,24($2)
	addu	$3,$3,-1
	sll	$3,$3,2
	andi	$3,$3,0x0fff
	sll	$3,$3,12
	addu	$2,$2,-1
	sll	$2,$2,2
	andi	$2,$2,0x0fff
	or	$2,$2,$4
	or	$3,$3,$2
	sw	$3,4($6)
.L13:
	.set	noreorder
	.set	nomacro
	beq	$fp,$0,.L109
	move	$3,$18
	.set	macro
	.set	reorder

	lw	$13,88($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$13,$0,.L109
	move	$5,$18
	.set	macro
	.set	reorder

	addu	$18,$18,8
	andi	$4,$fp,0x00ff
	sll	$4,$4,12
	andi	$2,$fp,0x007f
	sll	$2,$2,1
	li	$3,16777216			# 0x01000000
	or	$2,$2,$3
	or	$4,$4,$2
	sw	$4,0($5)
	sw	$13,4($5)
	move	$3,$18
.L109:
	li	$2,-570425344			# 0xde000000
	sw	$2,0($3)
	lw	$2,0($19)
	#nop
	sw	$2,4($3)
	lbu	$2,29($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L93
	addu	$18,$18,8
	.set	macro
	.set	reorder

	move	$3,$18
	addu	$18,$18,8
	li	$2,-667418624			# 0xd8380000
	ori	$2,$2,0x0002
	sw	$2,0($3)
	li	$2,64			# 0x00000040
	sw	$2,4($3)
.L93:
	lbu	$2,28($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L94
	move	$3,$18
	.set	macro
	.set	reorder

	addu	$18,$18,8
	li	$2,-667418624			# 0xd8380000
	ori	$2,$2,0x0002
	sw	$2,0($3)
	li	$2,64			# 0x00000040
	sw	$2,4($3)
.L94:
	move	$2,$18
	lw	$31,68($sp)
	lw	$fp,64($sp)
	lw	$23,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,72
	.set	macro
	.set	reorder

	.end	func_80062EA4
.Lfe1:
	.size	 func_80062EA4,.Lfe1-func_80062EA4
