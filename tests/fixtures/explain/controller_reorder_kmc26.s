	.set	reorder

.L2:
	li	$2,0x00000001		# 1 		#  32 movsi_internal2/3
	sw	$2,D_80036F80 		#  34 movsi_internal2/8
	jal	func_8002AAB0 		#  37 call_value_internal1
	move	$18,$2 		#  39 movdi_internal/1
	move	$19,$3
	.set	noreorder
