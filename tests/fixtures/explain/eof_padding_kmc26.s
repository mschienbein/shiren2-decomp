	.set	noreorder
	.set	nomacro
	j	$31 		#  82 return_internal
	mul.s	$f0,$f0,$f2 		#  69 mulsf3
	.set	macro
	.set	reorder

	.end	func_80030DDC
.Lfe8:
	.size	 func_80030DDC,.Lfe8-func_80030DDC
	.ident	"GCC: (GNU) 2.7.2"
