	.file	1 "/Users/mooki/Code/n64-experiments/projects/shiren2/scratch/omp/wave39/r6-candidate/stage/tree/src/units/overlay_136dc0/func_801EDFBC_13FEDC.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_801EDFBC_13FEDC
	.type	 func_801EDFBC_13FEDC,@function
	.ent	func_801EDFBC_13FEDC
func_801EDFBC_13FEDC:
	.frame	$sp,56,$31		# vars= 16, regs= 4/0, args= 24, extra= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,56
	sw	$31,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	.set	noreorder
	.set	nomacro
	jal	func_801EDBFC_13FB1C
	sw	$16,40($sp)
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

	lbu	$2,D_801F5CCD_147BED
	#nop
	addu	$3,$2,-59
	sltu	$2,$3,21
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L6
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lw	$2,.L72($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L72:
	.word	.L7
	.word	.L10
	.word	.L21
	.word	.L31
	.word	.L32
	.word	.L35
	.word	.L36
	.word	.L41
	.word	.L42
	.word	.L43
	.word	.L44
	.word	.L47
	.word	.L51
	.word	.L52
	.word	.L53
	.word	.L54
	.word	.L55
	.word	.L66
	.word	.L70
	.word	.L71
	.word	.L65
	.text
.L7:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$2,264($2)
	#nop
	bne	$2,$0,.L8
	jal	func_800485F8
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L8:
	jal	func_800485C0
	lw	$4,D_801476B8
	jal	func_800485DC
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L10:
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	lw	$3,D_801F51E8_147108
	#nop
	lw	$4,276($3)
	li	$2,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	beq	$4,$2,.L13
	slt	$2,$4,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L20
	li	$2,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$4,$0,.L12
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

	j	.L74
.L20:
	.set	noreorder
	.set	nomacro
	beq	$4,$2,.L14
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$4,$2,.L15
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

	j	.L74
.L12:
	lbu	$4,267($3)
	lbu	$5,271($3)
	jal	func_80045BC0
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L13:
	lbu	$4,267($3)
	lbu	$5,271($3)
	jal	func_80045BE0
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L14:
	lw	$4,272($3)
	lbu	$5,267($3)
	lbu	$6,271($3)
	jal	func_80045C00
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L15:
	lw	$2,264($3)
	#nop
	bne	$2,$0,.L16
	lbu	$4,271($3)
	jal	func_80045C84
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L16:
	lbu	$4,271($3)
	jal	func_80045C68
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L21:
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

	lw	$5,D_801F51E8_147108
	#nop
	lw	$3,268($5)
	#nop
	sw	$3,28($sp)
	lw	$4,272($5)
	addu	$17,$sp,24
	sw	$4,24($sp)
	lw	$3,4($17)
	#nop
	or	$3,$3,$4
	.set	noreorder
	.set	nomacro
	bne	$3,$0,.L22
	move	$18,$2
	.set	macro
	.set	reorder

	lbu	$4,267($5)
	jal	func_800AC244
	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L6
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$2,D_801476B8
	#nop
	lw	$3,0($2)
	#nop
	sw	$3,32($sp)
	lw	$2,4($2)
	addu	$5,$sp,32
	.set	noreorder
	.set	nomacro
	jal	func_801EDEF8_13FE18
	sw	$2,36($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L22:
	.set	noreorder
	.set	nomacro
	jal	func_800B4E7C
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,D_801F51E8_147108
	#nop
	lw	$2,264($3)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L26
	li	$4,215			# 0x000000d7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	move	$5,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L26:
	lbu	$4,267($3)
	jal	func_800AC244
	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$16,$0,.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	#nop
	lw	$5,276($2)
	.set	noreorder
	.set	nomacro
	jal	func_801EA810_13C730
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800AE674
	sw	$16,280($2)
	.set	macro
	.set	reorder

	la	$4,D_801F51F0_147110
	.set	noreorder
	.set	nomacro
	jal	func_80083C90
	move	$5,$2
	.set	macro
	.set	reorder

	andi	$2,$18,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L29
	move	$5,$16
	.set	macro
	.set	reorder

	li	$4,210			# 0x000000d2
	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	move	$6,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	li	$4,13			# 0x0000000d
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	li	$4,14			# 0x0000000e
	.set	macro
	.set	reorder

.L29:
	lbu	$3,1($16)
	li	$2,138			# 0x0000008a
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L76
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800ACD34
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$16
.L76:
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800AD7E0
	li	$6,1			# 0x00000001
	.set	macro
	.set	reorder

	li	$4,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_800B1CEC
	move	$5,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L31:
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
	#nop
	lw	$4,276($3)
	#nop
	sw	$4,16($sp)
	lw	$5,264($3)
	lw	$6,268($3)
	lw	$7,272($3)
	.set	noreorder
	.set	nomacro
	jal	func_801E7E48_139D68
	andi	$4,$2,0x00ff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L32:
	lw	$3,D_801F51E8_147108
	#nop
	lw	$2,268($3)
	#nop
	beq	$2,$0,.L33
	lw	$4,264($3)
	jal	func_80045A24
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L33:
	lw	$4,264($3)
	jal	func_80045AC8
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L35:
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
	move	$17,$2
	move	$4,$3
	addu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$3,4($5)
	.set	macro
	.set	reorder

	move	$4,$0
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	move	$16,$2
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	addu	$4,$sp,24
	lbu	$5,279($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E60B0_137FD0
	andi	$6,$17,0x00ff
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	addu	$7,$sp,24
	lhu	$4,266($2)
	lhu	$5,270($2)
	lhu	$6,274($2)
	andi	$16,$16,0x00ff
	.set	noreorder
	.set	nomacro
	jal	func_801E6000_137F20
	sw	$16,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L36:
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

	andi	$3,$2,0x00ff
	li	$2,1			# 0x00000001
	bne	$3,$2,.L37
	jal	func_801E7AD8_1399F8
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L37:
	bne	$3,$0,.L39
	jal	func_801E7ABC_1399DC
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L39:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,264($2)
	lw	$5,268($2)
	lw	$6,272($2)
	lw	$7,276($2)
	jal	func_801E8590_13A4B0
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L41:
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,264($2)
	lw	$5,268($2)
	lw	$6,272($2)
	jal	func_801EA91C_13C83C
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L42:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,264($2)
	lw	$5,268($2)
	jal	func_801EA600_13C520
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L43:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,264($2)
	jal	func_801EA504_13C424
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L44:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$2,264($2)
	#nop
	beq	$2,$0,.L45
	jal	func_80045A60
	lw	$2,D_801F51E8_147108
	.set	noreorder
	.set	nomacro
	j	.L6
	sb	$0,2553($2)
	.set	macro
	.set	reorder

.L45:
	jal	func_80045A84
	lw	$3,D_801F51E8_147108
	li	$2,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	j	.L6
	sb	$2,2553($3)
	.set	macro
	.set	reorder

.L47:
	lw	$5,D_801F51E8_147108
	#nop
	lw	$3,268($5)
	li	$2,2			# 0x00000002
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L48
	li	$4,1			# 0x00000001
	.set	macro
	.set	reorder

	move	$4,$0
	lw	$6,264($5)
	.set	noreorder
	.set	nomacro
	jal	func_801E5168_137088
	move	$5,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L48:
	lw	$6,264($5)
	sll	$5,$3,8
	.set	noreorder
	.set	nomacro
	jal	func_801E5168_137088
	subu	$5,$5,$3
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,272($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$4,$0,.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801E7A0C_13992C
	addu	$4,$4,-1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L51:
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,3			# 0x00000003
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

	lw	$3,D_801F51E8_147108
	#nop
	lw	$4,264($3)
	lw	$5,276($3)
	.set	noreorder
	.set	nomacro
	jal	func_801EC87C_13E79C
	andi	$6,$2,0x00ff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	andi	$2,$2,0xffff
	.set	macro
	.set	reorder

.L52:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,264($2)
	jal	func_801F3318_145238
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L53:
	lw	$2,D_801F51E8_147108
	#nop
	lhu	$4,266($2)
	lbu	$17,267($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E8D0C_13AC2C
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$5,D_801F51EC_14710C
	#nop
	lw	$3,4($5)
	move	$16,$2
	move	$4,$3
	addu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$3,4($5)
	.set	macro
	.set	reorder

	andi	$4,$16,0x00ff
	lw	$3,D_801F51E8_147108
	andi	$2,$2,0x00ff
	addu	$5,$3,264
	.set	noreorder
	.set	nomacro
	jal	func_801E63BC_1382DC
	sw	$2,264($3)
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	.L6
	sw	$17,264($2)
	.set	macro
	.set	reorder

.L54:
	lw	$3,D_801F51E8_147108
	#nop
	lbu	$2,267($3)
	.set	noreorder
	.set	nomacro
	j	.L6
	sb	$2,2457($3)
	.set	macro
	.set	reorder

.L55:
	jal	func_800AB134
	lw	$3,D_801F51E8_147108
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L6
	sw	$2,280($3)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800AE674
	move	$4,$2
	.set	macro
	.set	reorder

	la	$4,D_801F51F0_147110
	.set	noreorder
	.set	nomacro
	jal	func_80083C90
	move	$5,$2
	.set	macro
	.set	reorder

	li	$4,4			# 0x00000004
	.set	noreorder
	.set	nomacro
	jal	func_801E8CF4_13AC14
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801EA2D8_13C1F8
	li	$4,23			# 0x00000017
	.set	macro
	.set	reorder

	move	$4,$2
	bne	$4,$0,.L57
	lw	$2,D_801F51E8_147108
	#nop
	lw	$3,280($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$0,.L6
	li	$5,3			# 0x00000003
	.set	macro
	.set	reorder

	lw	$2,8($3)
	#nop
	lh	$4,8($2)
	lw	$2,12($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$3,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L57:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$5,280($2)
	jal	func_800CD538
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L61
	li	$4,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801E8CF4_13AC14
	li	$5,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L61:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$3,280($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$0,.L64
	li	$5,3			# 0x00000003
	.set	macro
	.set	reorder

	lw	$2,8($3)
	#nop
	lh	$4,8($2)
	lw	$2,12($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$3,$4
	.set	macro
	.set	reorder

.L64:
	li	$4,4			# 0x00000004
	.set	noreorder
	.set	nomacro
	jal	func_801E8CF4_13AC14
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	.set	noreorder
	.set	nomacro
	j	.L6
	sw	$0,280($2)
	.set	macro
	.set	reorder

.L65:
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,3			# 0x00000003
	.set	macro
	.set	reorder

	jal	func_801EAAA8_13C9C8
	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L66:
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	lbu	$3,D_801F5CCC_147BEC
	lbu	$2,271($2)
	#nop
	addu	$3,$3,$2
	sb	$3,D_801F5CCC_147BEC
	sll	$3,$3,24
	sra	$2,$3,24
	.set	noreorder
	.set	nomacro
	bgez	$2,.L67
	slt	$2,$2,8
	.set	macro
	.set	reorder

	sb	$0,D_801F5CCC_147BEC
	j	.L68
.L67:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L68
	li	$2,7			# 0x00000007
	.set	macro
	.set	reorder

	sb	$2,D_801F5CCC_147BEC
.L68:
	lw	$2,D_801F51E8_147108
	#nop
	lhu	$4,266($2)
	jal	func_801EB5D4_13D4F4
	j	.L75
.L70:
	.set	noreorder
	.set	nomacro
	jal	func_80046C30
	move	$4,$0
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	#nop
	lw	$5,264($2)
	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	li	$4,11			# 0x0000000b
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L74
	li	$2,4			# 0x00000004
	.set	macro
	.set	reorder

.L71:
	lw	$4,D_801F51EC_14710C
	jal	func_801EB53C_13D45C
.L75:
	sw	$2,D_801F51EC_14710C
.L6:
	li	$2,4			# 0x00000004
.L74:
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

	.end	func_801EDFBC_13FEDC
.Lfe1:
	.size	 func_801EDFBC_13FEDC,.Lfe1-func_801EDFBC_13FEDC
