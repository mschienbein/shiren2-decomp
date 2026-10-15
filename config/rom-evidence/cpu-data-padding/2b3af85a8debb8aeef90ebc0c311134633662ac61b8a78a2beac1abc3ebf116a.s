	.file	1 "/Users/mooki/Code/n64-experiments/projects/shiren2/scratch/omp/wave39/r4-candidate/stage/tree/src/units/overlay_1339f0/func_801E51F0_133D40.c"
	.version	"01.01"
gcc2_compiled.:
	.globl	D_801E7840_136390
	.data
	.align	2
	.type	 D_801E7840_136390,@object
	.size	 D_801E7840_136390,4
D_801E7840_136390:
	.word	0
	.text
	.align	2
	.globl	func_801E51F0_133D40
	.type	 func_801E51F0_133D40,@function
	.ent	func_801E51F0_133D40
func_801E51F0_133D40:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, extra= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,24
	sw	$31,16($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80042BA8
	li	$7,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$31,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	func_801E51F0_133D40
.Lfe1:
	.size	 func_801E51F0_133D40,.Lfe1-func_801E51F0_133D40
	.align	2
	.globl	func_801E520C_133D5C
	.type	 func_801E520C_133D5C,@function
	.ent	func_801E520C_133D5C
func_801E520C_133D5C:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, extra= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,24
	sw	$31,16($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80042BA8
	move	$7,$0
	.set	macro
	.set	reorder

	lw	$31,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	func_801E520C_133D5C
.Lfe2:
	.size	 func_801E520C_133D5C,.Lfe2-func_801E520C_133D5C
.section	.rodata
	.align	2
.LC0:
	.string	"contpak"
	.text
	.align	2
	.globl	func_801E5228_133D78
	.type	 func_801E5228_133D78,@function
	.ent	func_801E5228_133D78
func_801E5228_133D78:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, extra= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,24
	la	$4,.LC0
	li	$5,65536			# 0x00010000
	sw	$31,16($sp)
	.set	noreorder
	.set	nomacro
	jal	func_8006A8D8
	ori	$5,$5,0x2000
	.set	macro
	.set	reorder

	move	$3,$2
	sw	$3,D_801E8290_136DE0
	.set	noreorder
	.set	nomacro
	beq	$3,$0,.L4
	li	$2,36864			# 0x00009000
	.set	macro
	.set	reorder

	la	$4,D_801E82A8_136DF8
	addu	$2,$3,$2
	sw	$2,D_801E8294_136DE4
	.set	noreorder
	.set	nomacro
	jal	func_800265E0
	li	$5,512			# 0x00000200
	.set	macro
	.set	reorder

	la	$4,D_801E8298_136DE8
	.set	noreorder
	.set	nomacro
	jal	func_800265E0
	li	$5,16			# 0x00000010
	.set	macro
	.set	reorder

	la	$4,D_801E84A8_136FF8
	sw	$0,D_801E7840_136390
	.set	noreorder
	.set	nomacro
	jal	func_800265E0
	li	$5,17			# 0x00000011
	.set	macro
	.set	reorder

	lw	$2,D_801E8290_136DE0
	j	.L5
.L4:
	move	$2,$0
.L5:
	lw	$31,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	func_801E5228_133D78
.Lfe3:
	.size	 func_801E5228_133D78,.Lfe3-func_801E5228_133D78
	.align	2
	.globl	func_801E52BC_133E0C
	.type	 func_801E52BC_133E0C,@function
	.ent	func_801E52BC_133E0C
func_801E52BC_133E0C:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, extra= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	lw	$4,D_801E8290_136DE0
	subu	$sp,$sp,24
	sw	$31,16($sp)
	jal	func_8006A9D4
	lw	$31,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	func_801E52BC_133E0C
.Lfe4:
	.size	 func_801E52BC_133E0C,.Lfe4-func_801E52BC_133E0C
	.globl	D_801E7ED0_136A20
.section	.rodata
	.align	2
	.type	 D_801E7ED0_136A20,@object
	.size	 D_801E7ED0_136A20,4
D_801E7ED0_136A20:
	.string	"01"
	.zero	1
	.globl	D_801E7ED4_136A24
	.align	2
	.type	 D_801E7ED4_136A24,@object
	.size	 D_801E7ED4_136A24,8
D_801E7ED4_136A24:
	.string	"NSIJ"
	.zero	3
	.text
	.align	2
	.globl	func_801E52E0_133E30
	.type	 func_801E52E0_133E30,@function
	.ent	func_801E52E0_133E30
func_801E52E0_133E30:
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 24, extra= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,40
	sw	$16,24($sp)
	lw	$3,D_801E8294_136DE4
	li	$2,102			# 0x00000066
	sw	$31,32($sp)
	sw	$17,28($sp)
	sw	$2,D_801E7840_136390
	sw	$3,D_801E8290_136DE0
	.set	noreorder
	.set	nomacro
	jal	func_80058D70
	move	$16,$5
	.set	macro
	.set	reorder

	la	$17,D_801E8298_136DE8
	.set	noreorder
	.set	nomacro
	jal	func_80058E58
	move	$4,$17
	.set	macro
	.set	reorder

	la	$4,D_801E7ED0_136A20
	la	$5,D_801E7ED4_136A24
	jal	func_80058DA8
	move	$4,$17
	la	$5,D_801E84A8_136FF8
	move	$6,$16
	li	$7,1			# 0x00000001
	li	$2,10240			# 0x00002800
	.set	noreorder
	.set	nomacro
	jal	func_80058DC4
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,8($17)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L8
	move	$4,$17
	.set	macro
	.set	reorder

	sw	$2,D_801E7840_136390
	j	.L9
.L8:
	move	$5,$0
	li	$6,10240			# 0x00002800
	lw	$7,D_801E8290_136DE0
	li	$2,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_80058DE4
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$4,8($17)
	#nop
	beq	$4,$0,.L9
	sw	$4,D_801E7840_136390
.L9:
	jal	func_80058E04
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

	.end	func_801E52E0_133E30
.Lfe5:
	.size	 func_801E52E0_133E30,.Lfe5-func_801E52E0_133E30
	.align	2
	.globl	func_801E53BC_133F0C
	.type	 func_801E53BC_133F0C,@function
	.ent	func_801E53BC_133F0C
func_801E53BC_133F0C:
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 24, extra= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,40
	li	$2,101			# 0x00000065
	sll	$4,$4,5
	sw	$2,D_801E7840_136390
	la	$2,D_801E82A8_136DF8
	sw	$16,24($sp)
	addu	$16,$4,$2
	sw	$31,32($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80058D70
	sw	$17,28($sp)
	.set	macro
	.set	reorder

	la	$17,D_801E8298_136DE8
	.set	noreorder
	.set	nomacro
	jal	func_80058E58
	move	$4,$17
	.set	macro
	.set	reorder

	la	$4,D_801E7ED0_136A20
	la	$5,D_801E7ED4_136A24
	jal	func_80058DA8
	move	$4,$17
	addu	$5,$16,14
	addu	$6,$16,10
	lw	$2,0($16)
	move	$7,$0
	.set	noreorder
	.set	nomacro
	jal	func_80058DC4
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,8($17)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L12
	move	$4,$17
	.set	macro
	.set	reorder

	sw	$2,D_801E7840_136390
	j	.L13
.L12:
	lw	$7,D_801E8290_136DE0
	sw	$0,16($sp)
	lw	$6,0($16)
	.set	noreorder
	.set	nomacro
	jal	func_80058DE4
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$4,8($17)
	#nop
	beq	$4,$0,.L14
	sw	$4,D_801E7840_136390
	j	.L13
.L14:
	lw	$2,D_801E8290_136DE0
	#nop
	sw	$2,D_801E8294_136DE4
.L13:
	jal	func_80058E04
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,40
	.set	macro
	.set	reorder

	.end	func_801E53BC_133F0C
.Lfe6:
	.size	 func_801E53BC_133F0C,.Lfe6-func_801E53BC_133F0C
	.align	2
	.globl	func_801E54A4_133FF4
	.type	 func_801E54A4_133FF4,@function
	.ent	func_801E54A4_133FF4
func_801E54A4_133FF4:
	.frame	$sp,40,$31		# vars= 0, regs= 4/0, args= 24, extra= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,40
	sw	$18,32($sp)
	move	$18,$5
	li	$2,101			# 0x00000065
	sll	$4,$4,5
	sw	$2,D_801E7840_136390
	la	$2,D_801E82A8_136DF8
	sw	$16,24($sp)
	addu	$16,$4,$2
	sw	$31,36($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80058D70
	sw	$17,28($sp)
	.set	macro
	.set	reorder

	la	$17,D_801E8298_136DE8
	.set	noreorder
	.set	nomacro
	jal	func_80058E58
	move	$4,$17
	.set	macro
	.set	reorder

	la	$4,D_801E7ED0_136A20
	la	$5,D_801E7ED4_136A24
	jal	func_80058DA8
	move	$4,$17
	addu	$5,$16,14
	addu	$6,$16,10
	lw	$2,0($16)
	move	$7,$0
	.set	noreorder
	.set	nomacro
	jal	func_80058DC4
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,8($17)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L17
	move	$4,$17
	.set	macro
	.set	reorder

	sw	$2,D_801E7840_136390
	j	.L18
.L17:
	lw	$7,D_801E8290_136DE0
	sw	$0,16($sp)
	lw	$6,0($16)
	.set	noreorder
	.set	nomacro
	jal	func_80058DE4
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$4,8($17)
	#nop
	beq	$4,$0,.L19
	sw	$4,D_801E7840_136390
	j	.L18
.L19:
	lw	$2,D_801E8290_136DE0
	#nop
	sw	$2,D_801E8294_136DE4
	li	$2,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	beq	$18,$2,.L22
	li	$2,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$18,$2,.L23
	li	$6,32768			# 0x00008000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L21
	move	$2,$0
	.set	macro
	.set	reorder

.L22:
	.set	noreorder
	.set	nomacro
	j	.L26
	li	$6,10240			# 0x00002800
	.set	macro
	.set	reorder

.L23:
	li	$6,10240			# 0x00002800
	.set	noreorder
	.set	nomacro
	j	.L21
	move	$2,$6
	.set	macro
	.set	reorder

.L26:
	move	$2,$0
.L21:
	li	$4,134217728			# 0x08000000
	lw	$5,D_801E8294_136DE4
	.set	noreorder
	.set	nomacro
	jal	func_801E520C_133D5C
	or	$4,$2,$4
	.set	macro
	.set	reorder

.L18:
	jal	func_80058E04
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,40
	.set	macro
	.set	reorder

	.end	func_801E54A4_133FF4
.Lfe7:
	.size	 func_801E54A4_133FF4,.Lfe7-func_801E54A4_133FF4
	.align	2
	.globl	func_801E55DC_13412C
	.type	 func_801E55DC_13412C,@function
	.ent	func_801E55DC_13412C
func_801E55DC_13412C:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, extra= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,32
	li	$2,103			# 0x00000067
	sw	$16,16($sp)
	sll	$16,$4,5
	sw	$2,D_801E7840_136390
	la	$2,D_801E82A8_136DF8
	addu	$16,$16,$2
	sw	$31,24($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80058D70
	sw	$17,20($sp)
	.set	macro
	.set	reorder

	la	$17,D_801E8298_136DE8
	.set	noreorder
	.set	nomacro
	jal	func_80058E58
	move	$4,$17
	.set	macro
	.set	reorder

	addu	$4,$16,8
	.set	noreorder
	.set	nomacro
	jal	func_80058DA8
	addu	$5,$16,4
	.set	macro
	.set	reorder

	move	$4,$17
	addu	$5,$16,14
	.set	noreorder
	.set	nomacro
	jal	func_80058E3C
	addu	$6,$16,10
	.set	macro
	.set	reorder

	jal	func_80058E04
	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	func_801E55DC_13412C
.Lfe8:
	.size	 func_801E55DC_13412C,.Lfe8-func_801E55DC_13412C
	.align	2
	.globl	func_801E5654_1341A4
	.type	 func_801E5654_1341A4,@function
	.ent	func_801E5654_1341A4
func_801E5654_1341A4:
	.frame	$sp,40,$31		# vars= 0, regs= 4/0, args= 24, extra= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,40
	sw	$16,24($sp)
	move	$16,$4
	sw	$18,32($sp)
	move	$18,$5
	li	$4,134217728			# 0x08000000
	lw	$5,D_801E8290_136DE0
	li	$2,102			# 0x00000066
	sw	$31,36($sp)
	sw	$17,28($sp)
	sw	$2,D_801E7840_136390
	.set	noreorder
	.set	nomacro
	jal	func_801E51F0_133D40
	li	$6,32768			# 0x00008000
	.set	macro
	.set	reorder

	li	$2,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	beq	$16,$2,.L30
	li	$2,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$16,$2,.L31
	li	$6,32768			# 0x00008000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L29
	move	$3,$0
	.set	macro
	.set	reorder

.L30:
	.set	noreorder
	.set	nomacro
	j	.L37
	li	$6,10240			# 0x00002800
	.set	macro
	.set	reorder

.L31:
	li	$6,10240			# 0x00002800
	.set	noreorder
	.set	nomacro
	j	.L29
	move	$3,$6
	.set	macro
	.set	reorder

.L37:
	move	$3,$0
.L29:
	lw	$2,D_801E8290_136DE0
	#nop
	addu	$2,$2,$3
	sw	$2,D_801E8294_136DE4
	.set	noreorder
	.set	nomacro
	jal	func_80058D70
	move	$17,$6
	.set	macro
	.set	reorder

	la	$16,D_801E8298_136DE8
	.set	noreorder
	.set	nomacro
	jal	func_80058E58
	move	$4,$16
	.set	macro
	.set	reorder

	la	$4,D_801E7ED0_136A20
	la	$5,D_801E7ED4_136A24
	jal	func_80058DA8
	move	$4,$16
	la	$5,D_801E84A8_136FF8
	move	$6,$18
	li	$7,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_80058DC4
	sw	$17,16($sp)
	.set	macro
	.set	reorder

	lw	$2,8($16)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L34
	move	$4,$16
	.set	macro
	.set	reorder

	sw	$2,D_801E7840_136390
	j	.L35
.L34:
	move	$5,$0
	move	$6,$17
	lw	$7,D_801E8294_136DE4
	li	$2,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_80058DE4
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$4,8($16)
	#nop
	beq	$4,$0,.L35
	sw	$4,D_801E7840_136390
.L35:
	jal	func_80058E04
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,40
	.set	macro
	.set	reorder

	.end	func_801E5654_1341A4
.Lfe9:
	.size	 func_801E5654_1341A4,.Lfe9-func_801E5654_1341A4
	.align	2
	.globl	func_801E5788_1342D8
	.type	 func_801E5788_1342D8,@function
	.ent	func_801E5788_1342D8
func_801E5788_1342D8:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lw	$2,D_801E7840_136390
	#nop
	sltu	$3,$2,101
	.set	noreorder
	.set	nomacro
	beql	$3,$0,.L41
	move	$2,$0
	.set	macro
	.set	reorder

.L41:
	j	$31
	.end	func_801E5788_1342D8
.Lfe10:
	.size	 func_801E5788_1342D8,.Lfe10-func_801E5788_1342D8
	.align	2
	.globl	func_801E57A4_1342F4
	.type	 func_801E57A4_1342F4,@function
	.ent	func_801E57A4_1342F4
func_801E57A4_1342F4:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lw	$2,D_801E8294_136DE4
	j	$31
	.end	func_801E57A4_1342F4
.Lfe11:
	.size	 func_801E57A4_1342F4,.Lfe11-func_801E57A4_1342F4
	.globl	D_801E7EDC_136A2C
.section	.rodata
	.align	2
	.type	 D_801E7EDC_136A2C,@object
	.size	 D_801E7EDC_136A2C,14
D_801E7EDC_136A2C:
	.string	"kRvQh[yO\022\017\211;_"
	.text
	.align	2
	.globl	func_801E57B4_134304
	.type	 func_801E57B4_134304,@function
	.ent	func_801E57B4_134304
func_801E57B4_134304:
	.frame	$sp,48,$31		# vars= 0, regs= 7/0, args= 16, extra= 0
	.mask	0x803f0000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,48
	sw	$20,32($sp)
	move	$20,$4
	li	$5,16			# 0x00000010
	sw	$31,40($sp)
	sw	$21,36($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800265E0
	sw	$16,16($sp)
	.set	macro
	.set	reorder

	jal	func_80058D70
	la	$16,D_801E8298_136DE8
	.set	noreorder
	.set	nomacro
	jal	func_80058E58
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$2,12($16)
	li	$3,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	bne	$2,$3,.L44
	li	$17,15			# 0x0000000f
	.set	macro
	.set	reorder

	la	$2,D_801E82A8_136DF8
	addu	$18,$2,512
	addu	$19,$16,4
	li	$21,-1			# 0xffffffff
	addu	$16,$2,522
	addu	$18,$18,-32
.L53:
	addu	$4,$19,-4
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	func_80058E20
	sw	$17,0($19)
	.set	macro
	.set	reorder

	lw	$2,4($19)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L48
	addu	$16,$16,-32
	.set	macro
	.set	reorder

	lhu	$3,-2($16)
	lh	$2,D_801E7ED0_136A20
	la	$4,D_801E7ED4_136A24
	.set	noreorder
	.set	nomacro
	bnel	$3,$2,.L52
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$3,-6($16)
	lw	$2,0($4)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L52
	move	$4,$18
	.set	macro
	.set	reorder

	la	$5,D_801E7EDC_136A2C
	.set	noreorder
	.set	nomacro
	jal	func_80083D54
	addu	$4,$18,14
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L52
	move	$4,$18
	.set	macro
	.set	reorder

	lbu	$2,0($16)
	addu	$3,$20,$17
	addu	$2,$2,-16
	.set	noreorder
	.set	nomacro
	j	.L45
	sb	$2,0($3)
	.set	macro
	.set	reorder

.L48:
	move	$4,$18
.L52:
	.set	noreorder
	.set	nomacro
	jal	func_800265E0
	li	$5,32			# 0x00000020
	.set	macro
	.set	reorder

.L45:
	addu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	bne	$17,$21,.L53
	addu	$18,$18,-32
	.set	macro
	.set	reorder

	j	.L51
.L44:
	sw	$3,D_801E7840_136390
.L51:
	jal	func_80058E04
	lw	$31,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,48
	.set	macro
	.set	reorder

	.end	func_801E57B4_134304
.Lfe12:
	.size	 func_801E57B4_134304,.Lfe12-func_801E57B4_134304
	.align	2
	.globl	func_801E58EC_13443C
	.type	 func_801E58EC_13443C,@function
	.ent	func_801E58EC_13443C
func_801E58EC_13443C:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	la	$5,D_801E7EDC_136A2C
	la	$4,D_801E84A8_136FF8
	lwl	$2,0($5)
	lwr	$2,3($5)
	lwl	$3,4($5)
	lwr	$3,7($5)
	swl	$2,0($4)
	swr	$2,3($4)
	swl	$3,4($4)
	swr	$3,7($4)
	lwl	$2,8($5)
	lwr	$2,11($5)
	lb	$3,12($5)
	swl	$2,8($4)
	swr	$2,11($4)
	sb	$3,12($4)
	lb	$2,13($5)
	sb	$2,13($4)
	j	$31
	.end	func_801E58EC_13443C
.Lfe13:
	.size	 func_801E58EC_13443C,.Lfe13-func_801E58EC_13443C
	.align	2
	.globl	func_801E5940_134490
	.type	 func_801E5940_134490,@function
	.ent	func_801E5940_134490
func_801E5940_134490:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, extra= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,24
	sw	$31,20($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80058D70
	sw	$16,16($sp)
	.set	macro
	.set	reorder

	la	$16,D_801E8298_136DE8
	.set	noreorder
	.set	nomacro
	jal	func_80058E58
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$2,12($16)
	li	$3,1			# 0x00000001
	bne	$2,$3,.L56
	sw	$0,D_801E7840_136390
	j	.L57
.L56:
	sw	$3,D_801E7840_136390
.L57:
	jal	func_80058E04
	lw	$2,D_801E7840_136390
	lw	$31,20($sp)
	lw	$16,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	func_801E5940_134490
.Lfe14:
	.size	 func_801E5940_134490,.Lfe14-func_801E5940_134490
	.align	2
	.globl	func_801E59A4_1344F4
	.type	 func_801E59A4_1344F4,@function
	.ent	func_801E59A4_1344F4
func_801E59A4_1344F4:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lw	$4,D_801E7840_136390
	#nop
	sltu	$2,$4,105
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L78
	move	$3,$0
	.set	macro
	.set	reorder

	sll	$2,$4,2
	lw	$2,.L79($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L79:
	.word	.L59
	.word	.L72
	.word	.L65
	.word	.L66
	.word	.L78
	.word	.L59
	.word	.L67
	.word	.L74
	.word	.L74
	.word	.L59
	.word	.L71
	.word	.L70
	.word	.L70
	.word	.L70
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L78
	.word	.L59
	.word	.L76
	.word	.L76
	.word	.L77
	.word	.L59
	.text
.L65:
	.set	noreorder
	.set	nomacro
	j	.L59
	li	$3,1290			# 0x0000050a
	.set	macro
	.set	reorder

.L66:
	.set	noreorder
	.set	nomacro
	j	.L59
	li	$3,1291			# 0x0000050b
	.set	macro
	.set	reorder

.L67:
	.set	noreorder
	.set	nomacro
	j	.L59
	li	$3,1292			# 0x0000050c
	.set	macro
	.set	reorder

.L70:
	.set	noreorder
	.set	nomacro
	j	.L59
	li	$3,1293			# 0x0000050d
	.set	macro
	.set	reorder

.L71:
	.set	noreorder
	.set	nomacro
	j	$31
	li	$2,-10			# 0xfffffff6
	.set	macro
	.set	reorder

.L72:
	.set	noreorder
	.set	nomacro
	j	.L59
	li	$3,1295			# 0x0000050f
	.set	macro
	.set	reorder

.L74:
	.set	noreorder
	.set	nomacro
	j	.L59
	li	$3,1296			# 0x00000510
	.set	macro
	.set	reorder

.L76:
	.set	noreorder
	.set	nomacro
	j	.L59
	li	$3,1301			# 0x00000515
	.set	macro
	.set	reorder

.L77:
	.set	noreorder
	.set	nomacro
	j	.L59
	li	$3,1302			# 0x00000516
	.set	macro
	.set	reorder

.L78:
	li	$3,1299			# 0x00000513
.L59:
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$3
	.set	macro
	.set	reorder

	.end	func_801E59A4_1344F4
.Lfe15:
	.size	 func_801E59A4_1344F4,.Lfe15-func_801E59A4_1344F4
