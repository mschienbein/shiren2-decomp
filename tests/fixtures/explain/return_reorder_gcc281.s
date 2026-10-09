	.text
	.align	2
	.globl	func_800FFC08
	.type	 func_800FFC08,@function
	.ent	func_800FFC08
func_800FFC08:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lbu	$2,137($4)  # 11 zero_extendqisi2/2
	j	$31  # 23 return
	.end	func_800FFC08
.Lfe1:
	.size	 func_800FFC08,.Lfe1-func_800FFC08
