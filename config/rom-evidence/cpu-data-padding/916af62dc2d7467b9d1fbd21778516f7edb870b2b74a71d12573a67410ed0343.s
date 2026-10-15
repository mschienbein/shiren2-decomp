	.file	1 "source/src/units/main_14400/func_80092640.cpp"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_80092640
	.type	 func_80092640,@function
	.ent	func_80092640
func_80092640:
	.frame	$sp,208,$31		# vars= 168, regs= 4/0, args= 24, extra= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,208
	sw	$18,200($sp)
	move	$18,$4
	sw	$31,204($sp)
	sw	$17,196($sp)
	sw	$16,192($sp)
	lbu	$2,97($18)
	lw	$16,D_801476B8
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L42
	li	$2,9			# 0x00000009
	.set	macro
	.set	reorder

	lbu	$3,D_80142F24
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L42
	move	$17,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800E2044
	move	$4,$16
	.set	macro
	.set	reorder

	beq	$2,$0,.L18
	lw	$2,260($16)
	#nop
	sltu	$17,$2,1
.L18:
	beq	$17,$0,.L42
	.set	noreorder
	.set	nomacro
	jal	func_800A6DC8
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	beq	$4,$0,.L42
	li	$2,2			# 0x00000002
	.set	macro
	.set	reorder

	lbu	$5,3($4)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$5,$2,.L42
	li	$2,15			# 0x0000000f
	.set	macro
	.set	reorder

	lbu	$3,0($4)
	#nop
	bne	$3,$2,.L21
	lbu	$2,D_80142F18+3
	#nop
	andi	$3,$2,0x0003
	li	$2,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L226
	li	$2,1075			# 0x00000433
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$5,.L24
	li	$2,1077			# 0x00000435
	.set	macro
	.set	reorder

	li	$2,1076			# 0x00000434
.L24:
.L226:
	sh	$2,D_801401F0
	addu	$16,$sp,56
	.set	noreorder
	.set	nomacro
	jal	func_800953C0
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$16
	la	$5,D_801401F0
	la	$6,D_80138FA0
	la	$7,D_80138FB0
	la	$2,D_801521D0
	.set	noreorder
	.set	nomacro
	jal	func_80097240
	sw	$2,76($16)
	.set	macro
	.set	reorder

	move	$4,$16
	addu	$5,$sp,24
	li	$6,1			# 0x00000001
	move	$7,$0
	sw	$0,144($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800957C0
	sw	$0,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L28
	li	$2,48			# 0x00000030
	.set	macro
	.set	reorder

	lw	$3,24($sp)
	#nop
	bne	$3,$2,.L28
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DFC80
	move	$4,$2
	.set	macro
	.set	reorder

	la	$3,D_80151E38
	.set	noreorder
	.set	nomacro
	j	.L225
	sw	$3,76($16)
	.set	macro
	.set	reorder

.L28:
	la	$2,D_80151E38
	.set	noreorder
	.set	nomacro
	j	.L42
	sw	$2,132($sp)
	.set	macro
	.set	reorder

.L21:
	lbu	$3,10($16)
	lbu	$2,31($16)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L38
	move	$5,$0
	.set	macro
	.set	reorder

	lb	$2,5($4)
	#nop
	nor	$2,$0,$2
	beq	$2,$0,.L38
	lbu	$2,2($4)
	#nop
	andi	$2,$2,0x0020
	sltu	$5,$2,1
.L38:
	beq	$5,$0,.L42
	.set	noreorder
	.set	nomacro
	jal	func_80093398
	move	$4,$18
	.set	macro
	.set	reorder

	bne	$2,$0,.L225
.L42:
	lw	$2,132($18)
	#nop
	.set	noreorder
	.set	nomacro
	blez	$2,.L45
	move	$16,$0
	.set	macro
	.set	reorder

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

	lw	$5,136($18)
	.set	noreorder
	.set	nomacro
	jal	func_800CD090
	move	$4,$2
	.set	macro
	.set	reorder

	bltz	$2,.L45
	lw	$2,136($18)
	#nop
	lbu	$2,0($2)
	#nop
	xori	$2,$2,0x0009
	sltu	$16,$2,1
.L45:
	beq	$16,$0,.L47
	jal	func_80048440
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L50
	li	$16,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80049BF0
	move	$4,$0
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
	j	.L50
	li	$16,8			# 0x00000008
	.set	macro
	.set	reorder

.L47:
	.set	noreorder
	.set	nomacro
	jal	func_80094DA0
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80046190
	move	$4,$18
	.set	macro
	.set	reorder

	move	$16,$2
.L50:
	jal	func_800C9E10
	.set	noreorder
	.set	nomacro
	jal	func_800452C0
	move	$4,$2
	.set	macro
	.set	reorder

	addu	$3,$16,-6
	sltu	$2,$3,45
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L42
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lw	$2,.L222($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L222:
	.word	.L52
	.word	.L218
	.word	.L219
	.word	.L55
	.word	.L217
	.word	.L193
	.word	.L189
	.word	.L185
	.word	.L197
	.word	.L201
	.word	.L205
	.word	.L209
	.word	.L213
	.word	.L65
	.word	.L61
	.word	.L57
	.word	.L69
	.word	.L73
	.word	.L77
	.word	.L81
	.word	.L85
	.word	.L97
	.word	.L93
	.word	.L89
	.word	.L101
	.word	.L105
	.word	.L109
	.word	.L113
	.word	.L117
	.word	.L129
	.word	.L125
	.word	.L121
	.word	.L133
	.word	.L137
	.word	.L141
	.word	.L145
	.word	.L149
	.word	.L161
	.word	.L157
	.word	.L153
	.word	.L165
	.word	.L169
	.word	.L173
	.word	.L177
	.word	.L181
	.text
.L52:
	lw	$4,D_801476B8
	#nop
	lw	$2,260($4)
	#nop
	bne	$2,$0,.L53
	jal	func_800A6D40
	beq	$2,$0,.L53
	lw	$4,D_801476B8
	.set	noreorder
	.set	nomacro
	jal	func_800EC6A4
	move	$5,$2
	.set	macro
	.set	reorder

	beq	$2,$0,.L53
	.set	noreorder
	.set	nomacro
	jal	func_801E8A84
	li	$4,27			# 0x0000001b
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801E8A84
	li	$4,23			# 0x00000017
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800D92C0
	move	$4,$2
	.set	macro
	.set	reorder

	j	.L225
.L53:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800D90C0
	move	$4,$2
	.set	macro
	.set	reorder

	j	.L225
.L55:
	lw	$2,D_801476B8
	#nop
	lw	$2,260($2)
	#nop
	beq	$2,$0,.L56
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DA010
	move	$4,$2
	.set	macro
	.set	reorder

	j	.L225
.L56:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DF9A0
	move	$4,$2
	.set	macro
	.set	reorder

	j	.L225
.L57:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,4			# 0x00000004
	sb	$3,152($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L228
	addu	$5,$sp,152
	.set	macro
	.set	reorder

.L61:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,6			# 0x00000006
	sb	$3,153($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L228
	addu	$5,$sp,153
	.set	macro
	.set	reorder

.L65:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,2			# 0x00000002
	sb	$3,154($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L228
	addu	$5,$sp,154
	.set	macro
	.set	reorder

.L69:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	sb	$0,155($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L228
	addu	$5,$sp,155
	.set	macro
	.set	reorder

.L73:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,3			# 0x00000003
	sb	$3,156($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L228
	addu	$5,$sp,156
	.set	macro
	.set	reorder

.L77:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,1			# 0x00000001
	sb	$3,157($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L228
	addu	$5,$sp,157
	.set	macro
	.set	reorder

.L81:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,5			# 0x00000005
	sb	$3,158($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L228
	addu	$5,$sp,158
	.set	macro
	.set	reorder

.L85:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,7			# 0x00000007
	sb	$3,159($sp)
	move	$4,$2
	addu	$5,$sp,159
.L228:
	.set	noreorder
	.set	nomacro
	jal	func_800DEF90
	move	$6,$0
	.set	macro
	.set	reorder

	j	.L225
.L89:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,4			# 0x00000004
	sb	$3,160($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L229
	addu	$5,$sp,160
	.set	macro
	.set	reorder

.L93:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,6			# 0x00000006
	sb	$3,161($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L229
	addu	$5,$sp,161
	.set	macro
	.set	reorder

.L97:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,2			# 0x00000002
	sb	$3,162($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L230
	addu	$5,$sp,162
	.set	macro
	.set	reorder

.L101:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	sb	$0,163($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L229
	addu	$5,$sp,163
	.set	macro
	.set	reorder

.L105:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,3			# 0x00000003
	sb	$3,164($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L229
	addu	$5,$sp,164
	.set	macro
	.set	reorder

.L109:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,1			# 0x00000001
	sb	$3,165($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L229
	addu	$5,$sp,165
	.set	macro
	.set	reorder

.L113:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,5			# 0x00000005
	sb	$3,166($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L229
	addu	$5,$sp,166
	.set	macro
	.set	reorder

.L117:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,7			# 0x00000007
	sb	$3,167($sp)
	move	$4,$2
	addu	$5,$sp,167
.L229:
	.set	noreorder
	.set	nomacro
	jal	func_800DEF90
	li	$6,2			# 0x00000002
	.set	macro
	.set	reorder

	j	.L225
.L121:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,4			# 0x00000004
	sb	$3,168($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L230
	addu	$5,$sp,168
	.set	macro
	.set	reorder

.L125:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,6			# 0x00000006
	sb	$3,169($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L231
	addu	$5,$sp,169
	.set	macro
	.set	reorder

.L129:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,2			# 0x00000002
	sb	$3,170($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L231
	addu	$5,$sp,170
	.set	macro
	.set	reorder

.L133:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	sb	$0,171($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L231
	addu	$5,$sp,171
	.set	macro
	.set	reorder

.L137:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,3			# 0x00000003
	sb	$3,172($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L231
	addu	$5,$sp,172
	.set	macro
	.set	reorder

.L141:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,1			# 0x00000001
	sb	$3,173($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L231
	addu	$5,$sp,173
	.set	macro
	.set	reorder

.L145:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,5			# 0x00000005
	sb	$3,174($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L231
	addu	$5,$sp,174
	.set	macro
	.set	reorder

.L149:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,7			# 0x00000007
	sb	$3,175($sp)
	move	$4,$2
	addu	$5,$sp,175
.L231:
	.set	noreorder
	.set	nomacro
	jal	func_800DEF90
	li	$6,4			# 0x00000004
	.set	macro
	.set	reorder

	j	.L225
.L153:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,4			# 0x00000004
	sb	$3,176($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L232
	addu	$5,$sp,176
	.set	macro
	.set	reorder

.L157:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,6			# 0x00000006
	sb	$3,177($sp)
	move	$4,$2
	addu	$5,$sp,177
.L230:
	.set	noreorder
	.set	nomacro
	jal	func_800DEF90
	move	$6,$3
	.set	macro
	.set	reorder

	j	.L225
.L161:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,2			# 0x00000002
	sb	$3,178($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L232
	addu	$5,$sp,178
	.set	macro
	.set	reorder

.L165:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	sb	$0,179($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L232
	addu	$5,$sp,179
	.set	macro
	.set	reorder

.L169:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,3			# 0x00000003
	sb	$3,180($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L232
	addu	$5,$sp,180
	.set	macro
	.set	reorder

.L173:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,1			# 0x00000001
	sb	$3,181($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L232
	addu	$5,$sp,181
	.set	macro
	.set	reorder

.L177:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,5			# 0x00000005
	sb	$3,182($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	.L232
	addu	$5,$sp,182
	.set	macro
	.set	reorder

.L181:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,7			# 0x00000007
	sb	$3,183($sp)
	move	$4,$2
	addu	$5,$sp,183
.L232:
	.set	noreorder
	.set	nomacro
	jal	func_800DEF90
	li	$6,6			# 0x00000006
	.set	macro
	.set	reorder

	j	.L225
.L185:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,4			# 0x00000004
	sb	$3,184($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DA530
	li	$5,67108864			# 0x04000000
	.set	macro
	.set	reorder

	j	.L225
.L189:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,6			# 0x00000006
	sb	$3,185($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DA530
	li	$5,100663296			# 0x06000000
	.set	macro
	.set	reorder

	j	.L225
.L193:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,2			# 0x00000002
	sb	$3,186($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DA530
	li	$5,33554432			# 0x02000000
	.set	macro
	.set	reorder

	j	.L225
.L197:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	sb	$0,187($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DA530
	move	$5,$0
	.set	macro
	.set	reorder

	j	.L225
.L201:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,3			# 0x00000003
	sb	$3,188($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DA530
	li	$5,50331648			# 0x03000000
	.set	macro
	.set	reorder

	j	.L225
.L205:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,1			# 0x00000001
	sb	$3,189($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DA530
	li	$5,16777216			# 0x01000000
	.set	macro
	.set	reorder

	j	.L225
.L209:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,5			# 0x00000005
	sb	$3,190($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DA530
	li	$5,83886080			# 0x05000000
	.set	macro
	.set	reorder

	j	.L225
.L213:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	li	$3,7			# 0x00000007
	sb	$3,191($sp)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DA530
	li	$5,117440512			# 0x07000000
	.set	macro
	.set	reorder

	j	.L225
.L217:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DA620
	move	$4,$2
	.set	macro
	.set	reorder

	j	.L225
.L218:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DF8C0
	move	$4,$2
	.set	macro
	.set	reorder

	j	.L225
.L219:
	lbu	$3,D_80142F24
	li	$2,9			# 0x00000009
	beq	$3,$2,.L42
	jal	func_800C9E00
	.set	noreorder
	.set	nomacro
	jal	func_800D10C4
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80092E98
	move	$4,$18
	.set	macro
	.set	reorder

	beq	$2,$0,.L42
.L225:
	lw	$31,204($sp)
	lw	$18,200($sp)
	lw	$17,196($sp)
	lw	$16,192($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,208
	.set	macro
	.set	reorder

	.end	func_80092640
.Lfe1:
	.size	 func_80092640,.Lfe1-func_80092640
