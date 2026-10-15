	.file	1 "vt.cpp"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	f__4Base
	.type	 f__4Base,@function
	.ent	f__4Base
f__4Base:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	j	$31
	.end	f__4Base
.Lfe1:
	.size	 f__4Base,.Lfe1-f__4Base
	.align	2
	.globl	g__4Basei
	.type	 g__4Basei,@function
	.ent	g__4Basei
g__4Basei:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$5
	.set	macro
	.set	reorder

	.end	g__4Basei
.Lfe2:
	.size	 g__4Basei,.Lfe2-g__4Basei
	.align	2
	.globl	o__5Other
	.type	 o__5Other,@function
	.ent	o__5Other
o__5Other:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	j	$31
	.end	o__5Other
.Lfe3:
	.size	 o__5Other,.Lfe3-o__5Other
	.align	2
	.globl	__7Derived
	.type	 __7Derived,@function
	.ent	__7Derived
__7Derived:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	move	$2,$4
	la	$3,_vt.4Base
	sw	$3,4($2)
	la	$3,_vt.5Other
	sw	$3,12($2)
	la	$3,_vt.7Derived.5Other
	sw	$3,12($2)
	la	$3,_vt.7Derived
	.set	noreorder
	.set	nomacro
	j	$31
	sw	$3,4($2)
	.set	macro
	.set	reorder

	.end	__7Derived
.Lfe4:
	.size	 __7Derived,.Lfe4-__7Derived
	.align	2
	.globl	f__7Derived
	.type	 f__7Derived,@function
	.ent	f__7Derived
f__7Derived:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	j	$31
	.end	f__7Derived
.Lfe5:
	.size	 f__7Derived,.Lfe5-f__7Derived
	.align	2
	.globl	g__7Derivedi
	.type	 g__7Derivedi,@function
	.ent	g__7Derivedi
g__7Derivedi:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$2,$5,1
	.set	macro
	.set	reorder

	.end	g__7Derivedi
.Lfe6:
	.size	 g__7Derivedi,.Lfe6-g__7Derivedi
	.align	2
	.globl	h__7Derived
	.type	 h__7Derived,@function
	.ent	h__7Derived
h__7Derived:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	j	$31
	.end	h__7Derived
.Lfe7:
	.size	 h__7Derived,.Lfe7-h__7Derived
	.align	2
	.globl	o__7Derived
	.type	 o__7Derived,@function
	.ent	o__7Derived
o__7Derived:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	j	$31
	.end	o__7Derived
.Lfe8:
	.size	 o__7Derived,.Lfe8-o__7Derived
	.globl	_vt.7Derived.5Other
.section	.rodata
	.align	3
	.type	 _vt.7Derived.5Other,@object
	.size	 _vt.7Derived.5Other,16
_vt.7Derived.5Other:
	.half	-8
	.half	0
	.word	0
	.half	-8
	.half	0
	.word	o__7Derived
	.globl	_vt.7Derived
	.align	3
	.type	 _vt.7Derived,@object
	.size	 _vt.7Derived,32
_vt.7Derived:
	.half	0
	.half	0
	.word	0
	.half	0
	.half	0
	.word	f__7Derived
	.half	0
	.half	0
	.word	g__7Derivedi
	.half	0
	.half	0
	.word	h__7Derived
	.globl	_vt.5Other
	.align	3
	.type	 _vt.5Other,@object
	.size	 _vt.5Other,16
_vt.5Other:
	.half	0
	.half	0
	.word	0
	.half	0
	.half	0
	.word	o__5Other
	.globl	_vt.4Base
	.align	3
	.type	 _vt.4Base,@object
	.size	 _vt.4Base,32
_vt.4Base:
	.half	0
	.half	0
	.word	0
	.half	0
	.half	0
	.word	f__4Base
	.half	0
	.half	0
	.word	g__4Basei
	.half	0
	.half	0
	.word	__pure_virtual
