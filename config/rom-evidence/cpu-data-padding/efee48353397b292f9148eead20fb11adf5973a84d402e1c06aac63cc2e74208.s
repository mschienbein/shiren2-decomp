	.file	1 "source/src/units/main_14400/func_800D5A30.c"
	.version	"01.01"
gcc2_compiled.:
	.globl	D_80147FF4
	.data
	.align	2
	.type	 D_80147FF4,@object
	.size	 D_80147FF4,8
D_80147FF4:
	.word	0
	.word	0
	.text
	.align	2
	.globl	func_800D5A30
	.type	 func_800D5A30,@function
	.ent	func_800D5A30
func_800D5A30:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	sw	$4,D_80147FE0
	lbu	$2,8($4)
	sb	$0,D_80147FFC
	sb	$2,D_80147FF0
	lw	$3,0($4)
	lw	$5,4($4)
	sw	$3,D_80147FF4
	sw	$5,D_80147FF4+4
	j	$31
	.end	func_800D5A30
.Lfe1:
	.size	 func_800D5A30,.Lfe1-func_800D5A30
