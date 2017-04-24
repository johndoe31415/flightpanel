	ldr r1, =#0xdeadbeef
	ldr r2, =#0x20000000
	ldr r3, =#(0x20000 / 4)

	stackpaint_loop:
		cbz r3, stackpaint_end
		str r1, [r2]
		adds r2, r2, #4
		subs r3, r3, #1
	b stackpaint_loop
	stackpaint_end:
