	.file	1 "/Users/mooki/Code/n64-experiments/projects/shiren2/scratch/omp/wave39/r4-candidate/stage/tree/src/units/overlay_136dc0/func_801F3B5C_145A7C.cpp"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_801F3B5C_145A7C
	.type	 func_801F3B5C_145A7C,@function
	.ent	func_801F3B5C_145A7C
func_801F3B5C_145A7C:
	.frame	$sp,128,$31		# vars= 64, regs= 8/0, args= 32, extra= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	lw	$2,D_801F51E8_147108
	subu	$sp,$sp,128
	sw	$31,124($sp)
	sw	$22,120($sp)
	sw	$21,116($sp)
	sw	$20,112($sp)
	sw	$19,108($sp)
	sw	$18,104($sp)
	sw	$17,100($sp)
	sw	$16,96($sp)
	lhu	$4,266($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E8D0C_13AC2C
	move	$5,$0
	.set	macro
	.set	reorder

	andi	$20,$2,0x00ff
	.set	noreorder
	.set	nomacro
	jal	func_800A8CB0
	move	$4,$20
	.set	macro
	.set	reorder

	move	$17,$2
	.set	noreorder
	.set	nomacro
	jal	func_800A8974
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,.L14
	.set	noreorder
	.set	nomacro
	jal	func_800A8C00
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,0($17)
	#nop
	sw	$3,32($sp)
	lw	$3,4($17)
	#nop
	sw	$3,36($sp)
	lbu	$3,D_801F5CCD_147BED
	move	$21,$2
	addu	$3,$3,-96
	sltu	$2,$3,30
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L20
	move	$16,$17
	.set	macro
	.set	reorder

	sll	$2,$3,2
	lw	$2,.L87($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L87:
	.word	.L21
	.word	.L22
	.word	.L27
	.word	.L29
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L32
	.word	.L35
	.word	.L42
	.word	.L43
	.word	.L44
	.word	.L45
	.word	.L48
	.word	.L52
	.word	.L59
	.word	.L60
	.word	.L61
	.word	.L67
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L68
	.word	.L69
	.word	.L70
	.word	.L79
	.word	.L84
	.word	.L86
	.word	.L20
	.word	.L85
	.text
.L21:
	.set	noreorder
	.set	nomacro
	jal	func_801E6B4C_138A6C
	move	$4,$20
	.set	macro
	.set	reorder

	j	.L20
.L22:
	lw	$3,D_801F51E8_147108
	#nop
	lw	$2,276($3)
	#nop
	andi	$2,$2,0x0001
	beq	$2,$0,.L23
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lw	$3,36($sp)
	lw	$2,268($2)
	#nop
	addu	$3,$3,$2
	.set	noreorder
	.set	nomacro
	j	.L24
	sw	$3,44($sp)
	.set	macro
	.set	reorder

.L23:
	lw	$2,268($3)
	#nop
	sw	$2,44($sp)
.L24:
	lw	$3,D_801F51E8_147108
	#nop
	lw	$2,276($3)
	#nop
	andi	$2,$2,0x0002
	beq	$2,$0,.L25
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lw	$3,32($sp)
	lw	$2,272($2)
	#nop
	addu	$3,$3,$2
	.set	noreorder
	.set	nomacro
	j	.L26
	sw	$3,40($sp)
	.set	macro
	.set	reorder

.L25:
	lw	$2,272($3)
	#nop
	sw	$2,40($sp)
.L26:
	move	$4,$20
	move	$5,$0
	move	$6,$5
	.set	noreorder
	.set	nomacro
	jal	func_801E61B8_1380D8
	move	$7,$5
	.set	macro
	.set	reorder

	move	$4,$16
	addu	$5,$sp,40
	li	$2,4			# 0x00000004
	.set	noreorder
	.set	nomacro
	jal	func_800A58FC
	sb	$2,82($4)
	.set	macro
	.set	reorder

	lw	$2,40($sp)
	#nop
	sw	$2,16($sp)
	lw	$5,36($sp)
	lw	$6,32($sp)
	lw	$7,44($sp)
	.set	noreorder
	.set	nomacro
	jal	func_801E7200_139120
	andi	$4,$21,0x00ff
	.set	macro
	.set	reorder

	jal	func_801F33FC_14531C
	j	.L20
.L27:
	lw	$4,D_801F51E8_147108
	#nop
	lw	$3,268($4)
	li	$2,2			# 0x00000002
	bne	$3,$2,.L20
	lbu	$5,275($4)
	lbu	$6,274($4)
	lw	$7,276($4)
	.set	noreorder
	.set	nomacro
	jal	func_801E7610_139530
	andi	$4,$21,0x00ff
	.set	macro
	.set	reorder

	j	.L20
.L29:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_801F34D8_1453F8
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801E70D8_138FF8
	andi	$4,$21,0x00ff
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	move	$4,$17
	lw	$2,268($2)
	addu	$5,$sp,88
	andi	$2,$2,0x0007
	.set	noreorder
	.set	nomacro
	jal	func_800A665C
	sb	$2,88($sp)
	.set	macro
	.set	reorder

	j	.L20
.L32:
	.set	noreorder
	.set	nomacro
	jal	func_801E70D8_138FF8
	andi	$4,$21,0x00ff
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_801F34D8_1453F8
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	#nop
	lhu	$4,270($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E8D0C_13AC2C
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800A8CB0
	andi	$4,$2,0x00ff
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	jal	func_800A8974
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L20
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801F349C_1453BC
	move	$5,$16
	.set	macro
	.set	reorder

	jal	func_801F33FC_14531C
	j	.L20
.L35:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_801F34D8_1453F8
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801E70D8_138FF8
	andi	$4,$21,0x00ff
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	#nop
	lhu	$4,270($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E8D0C_13AC2C
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800A8CB0
	andi	$4,$2,0x00ff
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	jal	func_800A8974
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L20
	move	$5,$16
	.set	macro
	.set	reorder

	lbu	$16,8($17)
	.set	noreorder
	.set	nomacro
	jal	func_801F49BC_1468DC
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,D_801F51E8_147108
	#nop
	lw	$7,272($3)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$7,$0,.L38
	move	$4,$20
	.set	macro
	.set	reorder

	andi	$2,$2,0x0007
	sb	$2,89($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800A665C
	addu	$5,$sp,89
	.set	macro
	.set	reorder

	j	.L20
.L38:
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	func_801E7074_138F94
	move	$6,$2
	.set	macro
	.set	reorder

	j	.L20
.L42:
	.set	noreorder
	.set	nomacro
	jal	func_801E6A8C_1389AC
	andi	$4,$21,0x00ff
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	#nop
	lbu	$5,271($2)
	.set	noreorder
	.set	nomacro
	jal	func_800E039C
	move	$4,$16
	.set	macro
	.set	reorder

	li	$4,31			# 0x0000001f
	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	move	$5,$16
	.set	macro
	.set	reorder

	j	.L20
.L43:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$5,268($2)
	lw	$6,272($2)
	lw	$7,276($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E61B8_1380D8
	move	$4,$20
	.set	macro
	.set	reorder

	j	.L20
.L44:
	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	func_801E6394_1382B4
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	lbu	$6,8($17)
	lw	$2,D_801F51E8_147108
	lbu	$7,10($17)
	lw	$5,268($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E6708_138628
	move	$4,$20
	.set	macro
	.set	reorder

	j	.L20
.L45:
	lw	$3,D_801F51EC_14710C
	#nop
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	lw	$5,D_801F51EC_14710C
	#nop
	lw	$3,4($5)
	andi	$18,$2,0x00ff
	move	$4,$3
	addu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$3,4($5)
	.set	macro
	.set	reorder

	andi	$17,$2,0x00ff
	andi	$2,$18,0x0080
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L46
	li	$2,-256			# 0xffffff00
	.set	macro
	.set	reorder

	or	$18,$18,$2
.L46:
	andi	$2,$17,0x0080
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L47
	li	$2,-256			# 0xffffff00
	.set	macro
	.set	reorder

	or	$17,$17,$2
.L47:
	lw	$3,D_801F51EC_14710C
	#nop
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	lw	$5,D_801F51EC_14710C
	#nop
	lw	$3,4($5)
	andi	$16,$2,0x00ff
	move	$4,$3
	addu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$3,4($5)
	.set	macro
	.set	reorder

	sll	$4,$16,8
	andi	$2,$2,0x00ff
	lw	$3,D_801F51E8_147108
	addu	$16,$4,$2
	sw	$17,16($sp)
	lw	$2,272($3)
	move	$4,$20
	sw	$16,24($sp)
	sw	$2,20($sp)
	lw	$5,268($3)
	lw	$6,276($3)
	.set	noreorder
	.set	nomacro
	jal	func_801E72BC_1391DC
	move	$7,$18
	.set	macro
	.set	reorder

	j	.L20
.L48:
	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	func_801E6394_1382B4
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$3,D_801F51EC_14710C
	#nop
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	l.s	$f0,36($sp)
	cvt.s.w	$f0,$f0
	s.s	$f0,48($sp)
	l.s	$f0,32($sp)
	cvt.s.w	$f0,$f0
	lw	$3,D_801F51E8_147108
	andi	$6,$2,0x00ff
	s.s	$f0,56($sp)
	lw	$5,276($3)
	li	$2,255			# 0x000000ff
	.set	noreorder
	.set	nomacro
	beq	$5,$2,.L49
	addu	$2,$sp,48
	.set	macro
	.set	reorder

	sw	$2,16($sp)
	sw	$20,20($sp)
	lw	$4,268($3)
	lw	$7,272($3)
	.set	noreorder
	.set	nomacro
	jal	func_801E6D08_138C28
	addu	$5,$5,100
	.set	macro
	.set	reorder

	j	.L20
.L49:
	lbu	$2,8($17)
	#nop
	addu	$7,$2,4
	.set	noreorder
	.set	nomacro
	bgez	$7,.L51
	move	$5,$7
	.set	macro
	.set	reorder

	addu	$5,$2,11
.L51:
	andi	$5,$5,0x01f8
	subu	$5,$7,$5
	addu	$2,$sp,48
	sw	$2,16($sp)
	sw	$20,20($sp)
	lw	$4,268($3)
	lw	$7,272($3)
	jal	func_801E6D08_138C28
	j	.L20
.L52:
	.set	noreorder
	.set	nomacro
	jal	func_801E6A8C_1389AC
	move	$4,$20
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_801F34D8_1453F8
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$3,D_801F51E8_147108
	#nop
	lw	$2,276($3)
	#nop
	andi	$2,$2,0x0001
	beq	$2,$0,.L53
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lw	$3,36($sp)
	lw	$2,268($2)
	#nop
	addu	$3,$3,$2
	.set	noreorder
	.set	nomacro
	j	.L54
	sw	$3,44($sp)
	.set	macro
	.set	reorder

.L53:
	lw	$2,268($3)
	#nop
	sw	$2,44($sp)
.L54:
	lw	$3,D_801F51E8_147108
	#nop
	lw	$2,276($3)
	#nop
	andi	$2,$2,0x0002
	beq	$2,$0,.L55
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lw	$3,32($sp)
	lw	$2,272($2)
	#nop
	addu	$3,$3,$2
	.set	noreorder
	.set	nomacro
	j	.L56
	sw	$3,40($sp)
	.set	macro
	.set	reorder

.L55:
	lw	$2,272($3)
	#nop
	sw	$2,40($sp)
.L56:
	lw	$3,36($sp)
	lw	$2,44($sp)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L90
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,32($sp)
	lw	$2,40($sp)
	#nop
	beq	$3,$2,.L20
.L90:
	.set	noreorder
	.set	nomacro
	jal	func_800E5F38
	addu	$5,$sp,40
	.set	macro
	.set	reorder

	j	.L20
.L59:
	.set	noreorder
	.set	nomacro
	jal	func_801E75CC_1394EC
	move	$4,$20
	.set	macro
	.set	reorder

	j	.L20
.L60:
	.set	noreorder
	.set	nomacro
	jal	func_801E747C_13939C
	move	$4,$20
	.set	macro
	.set	reorder

	j	.L20
.L61:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$3,268($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$0,.L63
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L64
	li	$16,9			# 0x00000009
	.set	macro
	.set	reorder

	j	.L62
.L63:
	.set	noreorder
	.set	nomacro
	j	.L62
	li	$16,3			# 0x00000003
	.set	macro
	.set	reorder

.L64:
	li	$16,4			# 0x00000004
.L62:
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lbu	$4,10($17)
	lw	$6,272($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E7AA0_1399C0
	move	$5,$16
	.set	macro
	.set	reorder

	j	.L20
.L67:
	lw	$3,D_801F51EC_14710C
	#nop
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	lw	$3,D_801F51E8_147108
	andi	$2,$2,0x00ff
	sw	$2,16($sp)
	lw	$5,268($3)
	lw	$6,272($3)
	lw	$7,276($3)
	.set	noreorder
	.set	nomacro
	jal	func_801E6888_1387A8
	andi	$4,$21,0x00ff
	.set	macro
	.set	reorder

	j	.L20
.L68:
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lbu	$6,10($17)
	lw	$5,268($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E65EC_13850C
	andi	$4,$21,0x00ff
	.set	macro
	.set	reorder

	j	.L20
.L69:
	lw	$3,D_801F51EC_14710C
	#nop
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	lw	$5,D_801F51EC_14710C
	#nop
	lw	$3,4($5)
	andi	$22,$2,0x00ff
	move	$4,$3
	addu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$3,4($5)
	.set	macro
	.set	reorder

	sll	$4,$22,8
	lw	$5,D_801F51EC_14710C
	andi	$2,$2,0x00ff
	lw	$3,4($5)
	addu	$22,$4,$2
	move	$4,$3
	addu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$3,4($5)
	.set	macro
	.set	reorder

	lw	$5,D_801F51EC_14710C
	#nop
	lw	$3,4($5)
	andi	$18,$2,0x00ff
	move	$4,$3
	addu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$3,4($5)
	.set	macro
	.set	reorder

	lw	$5,D_801F51EC_14710C
	#nop
	lw	$3,4($5)
	andi	$19,$2,0x00ff
	move	$4,$3
	addu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$3,4($5)
	.set	macro
	.set	reorder

	lw	$5,D_801F51EC_14710C
	#nop
	lw	$3,4($5)
	andi	$17,$2,0x00ff
	move	$4,$3
	addu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$3,4($5)
	.set	macro
	.set	reorder

	li	$4,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	andi	$16,$2,0x00ff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,3			# 0x00000003
	.set	macro
	.set	reorder

	move	$4,$22
	move	$5,$18
	andi	$6,$21,0x00ff
	lw	$2,D_801F51E8_147108
	move	$7,$16
	l.s	$f0,268($2)
	cvt.s.w	$f0,$f0
	s.s	$f0,48($sp)
	l.s	$f0,272($2)
	cvt.s.w	$f0,$f0
	s.s	$f0,52($sp)
	l.s	$f0,276($2)
	cvt.s.w	$f0,$f0
	addu	$2,$sp,48
	s.s	$f0,56($sp)
	sw	$19,16($sp)
	.set	noreorder
	.set	nomacro
	jal	func_801E7C88_139BA8
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	func_801E6394_1382B4
	move	$5,$17
	.set	macro
	.set	reorder

	j	.L20
.L70:
	.set	noreorder
	.set	nomacro
	jal	func_801E6A8C_1389AC
	move	$4,$20
	.set	macro
	.set	reorder

	lw	$3,D_801F51E8_147108
	#nop
	lw	$2,276($3)
	#nop
	andi	$2,$2,0x0001
	beq	$2,$0,.L71
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lw	$3,36($sp)
	lw	$2,268($2)
	#nop
	addu	$3,$3,$2
	.set	noreorder
	.set	nomacro
	j	.L72
	sw	$3,44($sp)
	.set	macro
	.set	reorder

.L71:
	lw	$2,268($3)
	#nop
	sw	$2,44($sp)
.L72:
	lw	$3,D_801F51E8_147108
	#nop
	lw	$2,276($3)
	#nop
	andi	$2,$2,0x0002
	beq	$2,$0,.L73
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lw	$3,32($sp)
	lw	$2,272($2)
	#nop
	addu	$3,$3,$2
	.set	noreorder
	.set	nomacro
	j	.L74
	sw	$3,40($sp)
	.set	macro
	.set	reorder

.L73:
	lw	$2,272($3)
	#nop
	sw	$2,40($sp)
.L74:
	lw	$3,D_801F51EC_14710C
	#nop
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	move	$4,$16
	addu	$5,$sp,90
	andi	$2,$2,0x0007
	.set	noreorder
	.set	nomacro
	jal	func_800A665C
	sb	$2,90($sp)
	.set	macro
	.set	reorder

	lw	$3,36($sp)
	lw	$2,44($sp)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L91
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,32($sp)
	lw	$2,40($sp)
	#nop
	beq	$3,$2,.L20
.L91:
	.set	noreorder
	.set	nomacro
	jal	func_800E5F5C
	addu	$5,$sp,40
	.set	macro
	.set	reorder

	j	.L20
.L79:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_801F34D8_1453F8
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801E70D8_138FF8
	andi	$4,$21,0x00ff
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lbu	$5,8($17)
	lw	$7,272($2)
	lw	$6,268($2)
	.set	noreorder
	.set	nomacro
	bne	$7,$0,.L80
	andi	$2,$6,0x0007
	.set	macro
	.set	reorder

	sb	$2,91($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800A665C
	addu	$5,$sp,91
	.set	macro
	.set	reorder

	j	.L20
.L80:
	.set	noreorder
	.set	nomacro
	jal	func_801E7074_138F94
	move	$4,$20
	.set	macro
	.set	reorder

	j	.L20
.L84:
	.set	noreorder
	.set	nomacro
	jal	func_801F2E8C_144DAC
	move	$4,$17
	.set	macro
	.set	reorder

	j	.L20
.L85:
	move	$4,$17
	addu	$5,$sp,40
	sw	$0,44($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800A58FC
	sw	$0,40($sp)
	.set	macro
	.set	reorder

	li	$4,135			# 0x00000087
	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	move	$5,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800A59A4
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	j	.L20
.L86:
	lw	$3,D_801F51E8_147108
	lbu	$4,39($sp)
	lw	$5,D_801F51EC_14710C
	lhu	$2,270($3)
	#nop
	sh	$2,76($sp)
	lhu	$3,274($3)
	lbu	$2,35($sp)
	sb	$4,66($sp)
	sb	$2,67($sp)
	sh	$3,78($sp)
	lw	$2,4($5)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($5)
	.set	macro
	.set	reorder

	lw	$3,D_801F51EC_14710C
	sb	$2,80($sp)
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	lw	$3,D_801F51EC_14710C
	sb	$2,81($sp)
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	lw	$3,D_801F51EC_14710C
	sb	$2,82($sp)
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	lw	$3,D_801F51EC_14710C
	sb	$2,83($sp)
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	move	$4,$17
	sb	$2,84($sp)
	lbu	$2,8($4)
	addu	$5,$sp,64
	.set	noreorder
	.set	nomacro
	jal	func_801F1B58_143A78
	sb	$2,68($sp)
	.set	macro
	.set	reorder

.L20:
	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

.L14:
	lw	$31,124($sp)
	lw	$22,120($sp)
	lw	$21,116($sp)
	lw	$20,112($sp)
	lw	$19,108($sp)
	lw	$18,104($sp)
	lw	$17,100($sp)
	lw	$16,96($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,128
	.set	macro
	.set	reorder

	.end	func_801F3B5C_145A7C
.Lfe1:
	.size	 func_801F3B5C_145A7C,.Lfe1-func_801F3B5C_145A7C
