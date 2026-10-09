	.file	1 "/Users/mooki/Code/n64-experiments/projects/shiren2/scratch/omp/harness/merge-007/samples/func_8008C6B8.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_8008C6B8
	.type	 func_8008C6B8,@function
	.ent	func_8008C6B8
func_8008C6B8:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, extra= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,32  # 128 subsi3_internal
	sw	$16,16($sp)  # 134 movsi_internal2/7
	move	$16,$4  # 4 movsi_internal2/1
	sw	$17,20($sp)  # 132 movsi_internal2/7
	move	$17,$0  # 14 movsi_internal2/3
	move	$5,$17  # 19 movsi_internal2/1
	sw	$31,24($sp)  # 130 movsi_internal2/7
	.set	noreorder
	.set	nomacro
	jal	func_8006A810  # 23 call_value_internal1
	li	$6,32			# 0x00000020  # 21 movsi_internal2/3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_8008D6A0  # 30 call_internal1
	addu	$4,$16,16  # 28 addsi3_internal
	.set	macro
	.set	reorder

	li	$4,800			# 0x00000320  # 38 movsi_internal2/3
	li	$2,1			# 0x00000001  # 33 movqi_internal2/2
	.set	noreorder
	.set	nomacro
	jal	func_80091450  # 40 call_value_internal1
	sb	$2,0($16)  # 35 movqi_internal2/5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L7  # 48 branch_zero
	sw	$2,8($16)  # 46 movsi_internal2/7
	.set	macro
	.set	reorder

	move	$4,$2  # 60 movsi_internal2/1
	move	$5,$0  # 62 movsi_internal2/3
	.set	noreorder
	.set	nomacro
	jal	func_8006A810  # 66 call_value_internal1
	li	$6,800			# 0x00000320  # 64 movsi_internal2/3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80091450  # 71 call_value_internal1
	li	$4,40			# 0x00000028  # 69 movsi_internal2/3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L4  # 79 branch_zero
	sw	$2,28($16)  # 77 movsi_internal2/7
	.set	macro
	.set	reorder

.L7:
	.set	noreorder
	.set	nomacro
	j	.L3  # 85 jump
	li	$17,-1			# 0xffffffff  # 83 movsi_internal2/3
	.set	macro
	.set	reorder

.L4:
	move	$4,$2  # 91 movsi_internal2/1
	move	$5,$0  # 93 movsi_internal2/3
	.set	noreorder
	.set	nomacro
	jal	func_8006A810  # 97 call_value_internal1
	li	$6,40			# 0x00000028  # 95 movsi_internal2/3
	.set	macro
	.set	reorder

.L3:
	.set	noreorder
	.set	nomacro
	beq	$17,$0,.L8  # 104 branch_zero
	move	$2,$17  # 115 movsi_internal2/1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_8008C75C  # 110 call_internal1
	move	$4,$16  # 108 movsi_internal2/1
	.set	macro
	.set	reorder

	move	$2,$17  # 115 movsi_internal2/1
.L8:
	lw	$31,24($sp)  # 137 movsi_internal2/5
	lw	$17,20($sp)  # 139 movsi_internal2/5
	lw	$16,16($sp)  # 141 movsi_internal2/5
	#nop
	.set	noreorder
	.set	nomacro
	j	$31  # 145 return_internal
	addu	$sp,$sp,32  # 144 addsi3_internal
	.set	macro
	.set	reorder

	.end	func_8008C6B8
.Lfe1:
	.size	 func_8008C6B8,.Lfe1-func_8008C6B8
