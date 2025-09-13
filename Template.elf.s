
build/Template.elf:	file format elf32-littlearm

Disassembly of section .text:

03800000 <.text>:
 3800000: d4d4d4d4     	ldrble	sp, [r4], #1236
 3800004: d4d4d4d4     	ldrble	sp, [r4], #1236
 3800008: d4d4d4d4     	ldrble	sp, [r4], #1236
 380000c: d4d4d4d4     	ldrble	sp, [r4], #1236
 3800010: d4d4d4d4     	ldrble	sp, [r4], #1236
 3800014: d4d4d4d4     	ldrble	sp, [r4], #1236
 3800018: d4d4d4d4     	ldrble	sp, [r4], #1236
 380001c: d4d4d4d4     	ldrble	sp, [r4], #1236

03800020 <vexStartup>:
 3800020: e92d4830     	push	{r4, r5, r11, lr}
 3800024: e3040010     	movw	r0, #0x4010
 3800028: e3041010     	movw	r1, #0x4010
 380002c: e3400380     	movt	r0, #0x380
 3800030: e3401380     	movt	r1, #0x380
 3800034: e5900000     	ldr	r0, [r0]
 3800038: e5914000     	ldr	r4, [r1]
 380003c: e1540000     	cmp	r4, r0
 3800040: 2a00000a     	bhs	0x3800070 <vexStartup+0x50> @ imm = #0x28
 3800044: e2841004     	add	r1, r4, #4
 3800048: e1500001     	cmp	r0, r1
 380004c: 81a01000     	movhi	r1, r0
 3800050: e1e00004     	mvn	r0, r4
 3800054: e0810000     	add	r0, r1, r0
 3800058: e3c00003     	bic	r0, r0, #3
 380005c: e2805004     	add	r5, r0, #4
 3800060: e1a00004     	mov	r0, r4
 3800064: e1a01005     	mov	r1, r5
 3800068: eb000051     	bl	0x38001b4 <__aeabi_memclr8> @ imm = #0x144
 380006c: e0844005     	add	r4, r4, r5
 3800070: e3000000     	movw	r0, #0x0
 3800074: e3001000     	movw	r1, #0x0
 3800078: e3400000     	movt	r0, #0x0
 380007c: e3401000     	movt	r1, #0x0
 3800080: e5900000     	ldr	r0, [r0]
 3800084: e5911000     	ldr	r1, [r1]
 3800088: e1510000     	cmp	r1, r0
 380008c: 28bd8830     	pophs	{r4, r5, r11, pc}
 3800090: e3a00000     	mov	r0, #0
 3800094: e4840004     	str	r0, [r4], #4
 3800098: eafffffd     	b	0x3800094 <vexStartup+0x74> @ imm = #-0xc
 380009c: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000a0: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000a4: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000a8: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000ac: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000b0: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000b4: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000b8: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000bc: d4d4d4d4     	ldrble	sp, [r4], #1236

038000c0 <__aeabi_memset8>:
 38000c0: e3100003     	tst	r0, #3
 38000c4: e92d4010     	push	{r4, lr}
 38000c8: 0a000037     	beq	0x38001ac <__aeabi_memset8+0xec> @ imm = #0xdc
 38000cc: e3510000     	cmp	r1, #0
 38000d0: e2411001     	sub	r1, r1, #1
 38000d4: 0a000032     	beq	0x38001a4 <__aeabi_memset8+0xe4> @ imm = #0xc8
 38000d8: e202c0ff     	and	r12, r2, #255
 38000dc: e1a03000     	mov	r3, r0
 38000e0: ea000002     	b	0x38000f0 <__aeabi_memset8+0x30> @ imm = #0x8
 38000e4: e3510000     	cmp	r1, #0
 38000e8: e2411001     	sub	r1, r1, #1
 38000ec: 0a00002c     	beq	0x38001a4 <__aeabi_memset8+0xe4> @ imm = #0xb0
 38000f0: e4c3c001     	strb	r12, [r3], #1
 38000f4: e3130003     	tst	r3, #3
 38000f8: 1afffff9     	bne	0x38000e4 <__aeabi_memset8+0x24> @ imm = #-0x1c
 38000fc: e3510003     	cmp	r1, #3
 3800100: 9a000020     	bls	0x3800188 <__aeabi_memset8+0xc8> @ imm = #0x80
 3800104: e202e0ff     	and	lr, r2, #255
 3800108: e18ee40e     	orr	lr, lr, lr, lsl #8
 380010c: e351000f     	cmp	r1, #15
 3800110: e18ee80e     	orr	lr, lr, lr, lsl #16
 3800114: 9a000010     	bls	0x380015c <__aeabi_memset8+0x9c> @ imm = #0x40
 3800118: e1a04001     	mov	r4, r1
 380011c: e283c010     	add	r12, r3, #16
 3800120: e2444010     	sub	r4, r4, #16
 3800124: e354000f     	cmp	r4, #15
 3800128: e50ce010     	str	lr, [r12, #-0x10]
 380012c: e50ce00c     	str	lr, [r12, #-0xc]
 3800130: e50ce008     	str	lr, [r12, #-0x8]
 3800134: e50ce004     	str	lr, [r12, #-0x4]
 3800138: e28cc010     	add	r12, r12, #16
 380013c: 8afffff7     	bhi	0x3800120 <__aeabi_memset8+0x60> @ imm = #-0x24
 3800140: e241c010     	sub	r12, r1, #16
 3800144: e3ccc00f     	bic	r12, r12, #15
 3800148: e201100f     	and	r1, r1, #15
 380014c: e28cc010     	add	r12, r12, #16
 3800150: e3510003     	cmp	r1, #3
 3800154: e083300c     	add	r3, r3, r12
 3800158: 9a00000a     	bls	0x3800188 <__aeabi_memset8+0xc8> @ imm = #0x28
 380015c: e1a04003     	mov	r4, r3
 3800160: e1a0c001     	mov	r12, r1
 3800164: e24cc004     	sub	r12, r12, #4
 3800168: e35c0003     	cmp	r12, #3
 380016c: e484e004     	str	lr, [r4], #4
 3800170: 8afffffb     	bhi	0x3800164 <__aeabi_memset8+0xa4> @ imm = #-0x14
 3800174: e241c004     	sub	r12, r1, #4
 3800178: e3ccc003     	bic	r12, r12, #3
 380017c: e28cc004     	add	r12, r12, #4
 3800180: e083300c     	add	r3, r3, r12
 3800184: e2011003     	and	r1, r1, #3
 3800188: e3510000     	cmp	r1, #0
 380018c: 120220ff     	andne	r2, r2, #255
 3800190: 10831001     	addne	r1, r3, r1
 3800194: 0a000002     	beq	0x38001a4 <__aeabi_memset8+0xe4> @ imm = #0x8
 3800198: e4c32001     	strb	r2, [r3], #1
 380019c: e1530001     	cmp	r3, r1
 38001a0: 1afffffc     	bne	0x3800198 <__aeabi_memset8+0xd8> @ imm = #-0x10
 38001a4: e8bd4010     	pop	{r4, lr}
 38001a8: e12fff1e     	bx	lr
 38001ac: e1a03000     	mov	r3, r0
 38001b0: eaffffd1     	b	0x38000fc <__aeabi_memset8+0x3c> @ imm = #-0xbc

038001b4 <__aeabi_memclr8>:
 38001b4: e3a02000     	mov	r2, #0
 38001b8: eaffffc0     	b	0x38000c0 <__aeabi_memset8> @ imm = #-0x100
